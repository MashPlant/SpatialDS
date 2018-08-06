//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_BRUTE_HPP
#define SPATIALDS_BRUTE_HPP

#include <vector>
#include "../SpatialDS.hpp"

template<typename T>
struct SpatialDS<T, BruteTag>
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
		const int size = static_cast<int>(data.size());
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
			for (int j = i + 1; j < size; ++j)
				trait_type::check_collision(*data[i], *data[j]);
	}
};

#endif //SPATIALDS_BRUTE_HPP
