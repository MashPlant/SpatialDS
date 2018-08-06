//
// Created by mashplant on 18-8-4.
//

#ifndef SPATIALDS_POWSPLITTREE_HPP
#define SPATIALDS_POWSPLITTREE_HPP

#include <vector>
#include <array>
#include <memory>
#include "../SpatialDS.hpp"

namespace detail
{
	// non-leaf
	// child is binary-indexed
	// if any of p[k] intersect with mid[k], then p belong to self
	// else if mid[k] < p[k], then the k-th bit of p's corresponding child is 1
	template<typename T, int Depth, int Dim>
	struct PowSplitTreeNode
	{
		using trait_type = PointTraits<T>;
		using float_type = typename trait_type::float_type;
		using pointer = typename trait_type::pointer;
		using vec = std::array<float_type, Dim>;
		vec mid;
		std::vector<pointer> self;
		PowSplitTreeNode<T, Depth - 1, Dim> child[1 << Dim];

		void set_mid(vec min, vec max)
		{
			vec step;
			for (int i = 0; i < Dim; ++i)
				step[i] = (mid[i] = (min[i] + max[i]) / 2) - min[i];
			for (int i = 0; i < 1 << Dim; ++i)
			{
				vec ch_min = mid, ch_max = mid;
				for (int j = 0; j < Dim; ++j)
					i & (1 << j) ? ch_max[j] += step[j] : ch_min[j] -= step[j];
				child[i].set_mid(ch_min, ch_max);
			}
		}

		template<int Cur>
		void query(pointer p, int bit)
		{
			if constexpr (Cur == Dim)
				return void(child[bit].query(p));
			else
			{
				float_type pos = trait_type::template get<Cur>(*p), r = trait_type::radius(*p);
				if (pos - r <= mid[Cur])
					query<Cur + 1>(p, bit);
				if (pos + r >= mid[Cur])
					query<Cur + 1>(p, bit | (1 << Cur));
			}
		}

		void query(pointer p)
		{
			for (auto pt : self)
				if (pt != p)
					trait_type::check_collision(*pt, *p);
			query<0>(p, 0);
		}

		void insert(pointer p)
		{
			float_type r = trait_type::radius(*p);
			int which = 0;
			for (int i = 0; i < Dim; ++i)
				if (trait_type::get(*p, i) - r <= mid[i] && mid[i] <= trait_type::get(*p, i) + r)
					return void(self.push_back(p));
				else
					which |= (mid[i] < trait_type::get(*p, i)) << i;
			child[which].insert(p);
		}

		void clear()
		{
			self.clear();
			for (auto &ch : child)
				ch.clear();
		}
	};

	// leaf
	template<typename T, int Dim>
	struct PowSplitTreeNode<T, 0, Dim>
	{
		using trait_type = PointTraits<T>;
		using float_type = typename trait_type::float_type;
		using pointer = typename trait_type::pointer;
		using vec = std::array<float_type, Dim>;

		vec mid;
		std::vector<pointer> self;

		void set_mid(vec min, vec max)
		{
			for (int i = 0; i < Dim; ++i)
				mid[i] = (min[i] + max[i]) / 2;
		}

		void query(pointer p)
		{
			for (auto pt : self)
				if (pt != p)
					trait_type::check_collision(*pt, *p);
		}

		void insert(pointer p)
		{ self.push_back(p); }

		void clear()
		{ self.clear(); }
	};
}

template<typename T>
struct SpatialDS<T, PowSplitTreeTag>
{
	using trait_type = PointTraits<T>;
	using float_type = typename trait_type::float_type;
	using value_type = typename trait_type::value_type;
	using reference = typename trait_type::reference;
	using const_reference = typename trait_type::const_reference;
	using pointer = typename trait_type::pointer;
	using const_pointer = typename trait_type::const_pointer;

	const static int dim = trait_type::dim;
	const static int split_num = 1 << dim;
	const static int max_depth = 4;

	template<int Depth>
	using node = detail::PowSplitTreeNode<T, Depth, dim>;
	using max_node = node<max_depth>;

	// use pointer instead of entity to avoid this object being too big
	std::unique_ptr<max_node> root = std::make_unique<max_node>();
	std::vector<pointer> data;

	template<typename InputIt>
	void add_points(InputIt first, InputIt last)
	{
		data.reserve(data.size() + std::distance(first, last));
		for (; first != last; ++first)
			data.push_back(trait_type::address(*first));
		typename max_node::vec min, max;
		for (int i = 0; i < dim; ++i)
		{
			auto[min_elem, max_elem] = std::minmax_element(data.begin(), data.end(), [i](pointer lhs, pointer rhs)
			{ return trait_type::get(*lhs, i) < trait_type::get(*rhs, i); });
			min[i] = trait_type::get(**min_elem, i);
			max[i] = trait_type::get(**max_elem, i);
		}
		root->set_mid(min, max);
	}

	void update()
	{
		root->clear();
		for (auto p : data)
			root->insert(p);
		par::for_each(data.begin(), data.end(), [this](pointer p)
		{ root->query(p); });
	}
};

#endif //SPATIALDS_POWSPLITTREE_HPP
