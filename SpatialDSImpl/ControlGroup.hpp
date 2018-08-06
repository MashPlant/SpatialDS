//
// Created by mashplant on 18-8-6.
//

#ifndef SPATIALDS_CONTROLGROUP_HPP
#define SPATIALDS_CONTROLGROUP_HPP

#include "../SpatialDS.hpp"

template<typename T>
struct SpatialDS<T, ControlGroupTag>
{
	using trait_type = PointTraits<T>;
	using float_type = typename trait_type::float_type;
	using value_type = typename trait_type::value_type;
	using reference = typename trait_type::reference;
	using const_reference = typename trait_type::const_reference;
	using pointer = typename trait_type::pointer;
	using const_pointer = typename trait_type::const_pointer;

	template<typename InputIt>
	void add_points(InputIt first, InputIt last)
	{}

	void update()
	{}
};

#endif //SPATIALDS_CONTROLGROUP_HPP
