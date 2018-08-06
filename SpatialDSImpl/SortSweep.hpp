//
// Created by mashplant on 18-8-3.
//

#ifndef SPATIALDS_SORTSWEEP_HPP
#define SPATIALDS_SORTSWEEP_HPP

#include <vector>
#include "../SpatialDS.hpp"

template<typename T>
struct SpatialDS<T, SortSweepTag>
{
	using trait_type = PointTraits<T>;
	using float_type = typename trait_type::float_type;
	using value_type = typename trait_type::value_type;
	using reference = typename trait_type::reference;
	using const_reference = typename trait_type::const_reference;
	using pointer = typename trait_type::pointer;
	using const_pointer = typename trait_type::const_pointer;

	std::vector<pointer> data;

	template<typename InputIt>
	void add_points(InputIt first, InputIt last)
	{
		data.reserve(data.size() + std::distance(first, last));
		for (; first != last; ++first)
			data.push_back(trait_type::address(*first));
	}

	void update()
	{
		std::sort(data.begin(), data.end(), [](pointer lhs, pointer rhs)
		{
			return trait_type::template get<0>(*lhs) - trait_type::radius(*lhs)
				   < trait_type::template get<0>(*rhs) - trait_type::radius(*rhs);
		});
		const int size = static_cast<int>(data.size());
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			float_type right = trait_type::template get<0>(*data[i]) + trait_type::radius(*data[i]);
			for (int j = i + 1; j < size; ++j)
			{
				if (right < trait_type::template get<0>(*data[j]) - trait_type::radius(*data[j]))
					break;
				trait_type::check_collision(*data[i], *data[j]);
			}
		}
	}
};

#endif //SPATIALDS_SORTSWEEP_HPP
