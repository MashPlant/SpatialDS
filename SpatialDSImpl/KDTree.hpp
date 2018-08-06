//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_KDTREE_HPP
#define SPATIALDS_KDTREE_HPP

#include <vector>
#include "../SpatialDS.hpp"


template<typename T>
struct SpatialDS<T, KDTreeTag>
{
	using trait_type = PointTraits<T>;
	using float_type = typename trait_type::float_type;
	using value_type = typename trait_type::value_type;
	using reference = typename trait_type::reference;
	using const_reference = typename trait_type::const_reference;
	using pointer = typename trait_type::pointer;
	using const_pointer = typename trait_type::const_pointer;

	const static int dim = trait_type::dim;

	// lb = index of node
	// l = -1 & r = -1 => null
	struct Node
	{
		float_type split;
		int ub, l, r;
	};

	int root;
	std::vector<Node> tree; // implicit tree structure
	std::vector<pointer> data;

	template<typename InputIt>
	void add_points(InputIt first, InputIt last)
	{
		data.reserve(data.size() + std::distance(first, last));
		for (; first != last; ++first)
			data.push_back(trait_type::address(*first));
		tree.resize(data.size());
	}

	void update()
	{
		build<0>(root, 0, static_cast<int>(data.size()));
		par::for_each(data.begin(), data.end(), [this](pointer p)
		{ query<0>(root, p); });
	}

	template<int Split>
	void build(int &rt, int l, int r)
	{
		if (l >= r)
			return void(rt = -1);
		int mid = (l + r) >> 1;
		std::nth_element(data.begin() + l, data.begin() + mid, data.begin() + r, [](pointer lhs, pointer rhs)
		{ return trait_type::template get<Split>(*lhs) < trait_type::template get<Split>(*rhs); });
		float_type split = trait_type::template get<Split>(*data[mid]);
		// note: std::partition: all elem where f(elem) return true will be put front
		// return value: first elem where f(elem) return false
		int left = std::partition(data.begin() + l, data.begin() + mid, [split](pointer p)
		{ return split > trait_type::template get<Split>(*p) + trait_type::radius(*p); }) - data.begin();
		int right = std::partition(data.begin() + mid + 1, data.begin() + r, [split](pointer p)
		{ return split > trait_type::template get<Split>(*p) - trait_type::radius(*p); }) - data.begin();
		rt = left;
		tree[rt].ub = right;
		tree[rt].split = split;
		build<(Split + 1) % dim>(tree[rt].l, l, left);
		build<(Split + 1) % dim>(tree[rt].r, right, r);
	}

	template<int Split>
	void query(int rt, pointer p)
	{
		if (rt == -1)
			return;
		Node x = tree[rt];
		for (int i = rt; i < x.ub; ++i)
			if (p != data[i])
				trait_type::check_collision(*p, *data[i]);
		float_type pos = trait_type::template get<Split>(*p), radius = trait_type::radius(*p);
		if (pos - radius <= x.split)
			query<(Split + 1) % dim>(x.l, p);
		if (pos + radius >= x.split)
			query<(Split + 1) % dim>(x.r, p);
	}
};

#endif //SPATIALDS_KDTREE_HPP
