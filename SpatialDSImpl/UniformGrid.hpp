//
// Created by mashplant on 18-8-5.
//

#ifndef SPATIALDS_UNIFORMGRID_HPP
#define SPATIALDS_UNIFORMGRID_HPP

#include <vector>
#include <memory>
#include "../SpatialDS.hpp"

template<typename T>
struct SpatialDS<T, UniformGridTag>
{
	using trait_type = PointTraits<T>;
	using float_type = typename trait_type::float_type;
	using value_type = typename trait_type::value_type;
	using reference = typename trait_type::reference;
	using const_reference = typename trait_type::const_reference;
	using pointer = typename trait_type::pointer;
	using const_pointer = typename trait_type::const_pointer;

	const static int dim = trait_type::dim;
	const static size_t hash_base = 19260817;

	float_type inv_grid; // mul is (much) faster than div
	size_t bucket_num;
	std::unique_ptr<std::vector<pointer>[]> bucket;
	std::vector<pointer> data;

	// well, I must write the init here
	// otherwise g++ will ICE
	SpatialDS() : inv_grid{0}, bucket_num{1024}, bucket{std::make_unique<std::vector<pointer>[]>(bucket_num)}
	{}

	template<typename InputIt>
	void add_points(InputIt first, InputIt last)
	{
		data.reserve(data.size() + std::distance(first, last));
		for (; first != last; ++first)
			data.push_back(trait_type::address(*first));
		inv_grid =
				1 / (2 * trait_type::radius(**std::max_element(data.begin(), data.end(), [](pointer lhs, pointer rhs)
				{ return trait_type::radius(*lhs) < trait_type::radius(*rhs); })));
		for (bucket_num = 1024; bucket_num < data.size(); bucket_num <<= 1);
		bucket = std::make_unique<std::vector<pointer>[]>(bucket_num);
	}

	void update()
	{
		for (int i = 0; i < bucket_num; ++i)
			bucket[i].clear();
		std::for_each(data.begin(), data.end(), [this](pointer p)
		{
			work<0>(p, 0, [p](std::vector<pointer> &v)
			{ v.push_back(p); });
		});
		par::for_each(data.begin(), data.end(), [this](pointer p)
		{
			work<0>(p, 0, [p](const std::vector<pointer> &v)
			{
				for (auto pt : v)
					if (pt != p)
						trait_type::check_collision(*pt, *p);
			});
		});
	}

	template<int Cur, typename Func>
	void work(pointer p, size_t hash, Func f)
	{
		if constexpr (Cur == dim)
			f(bucket[hash & (bucket_num - 1)]);
		else
		{
			hash *= hash_base;
			float_type pos = trait_type::template get<Cur>(*p), r = trait_type::radius(*p);
			size_t center = static_cast<size_t>(pos * inv_grid), max, min;
			work<Cur + 1>(p, hash + center, f);
			if (center != (max = static_cast<size_t>((pos + r) * inv_grid)))
				work<Cur + 1>(p, hash + max, f);
			else if (center != (min = static_cast<size_t>((pos - r) * inv_grid)))
				work<Cur + 1>(p, hash + min, f);
		}
	}
};

#endif //SPATIALDS_UNIFORMGRID_HPP
