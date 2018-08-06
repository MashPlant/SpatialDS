//
// Created by mashplant on 18-8-2.
//

#include "SpatialDS.hpp"
#include "SpatialDSImpl/Brute.hpp"
#include "SpatialDSImpl/KDTree.hpp"
#include "SpatialDSImpl/SortSweep.hpp"
#include "SpatialDSImpl/PowSplitTree.hpp"
#include "SpatialDSImpl/UniformGrid.hpp"
#include "Point.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <array>

Float rand_float(Float l, Float r)
{ return l + (r - l) * (static_cast<Float>(rand()) / RAND_MAX); }

template<typename DSTag>
void test(const char *name)
{
	using Point = Point3;
	using Vec = Vec3;
	const int N = 2000;
	const Float min_pos = -100, max_pos = 100;
	const Float min_v = -100, max_v = 100;
	const Float min_r = 1.25f, max_r = 7.5f;
	const int fps = 60, all = 2000;
	const Float spf = 1.0f / fps;

	auto now = std::chrono::high_resolution_clock::now;
	auto beg = now();
	srand(19260817);
	std::vector<Point> v;
	for (int _ = 0; _ < N; ++_)
		v.push_back(Point(Vec::rand([=]()
									{ return rand_float(min_pos, max_pos); }),
						  Vec::rand([=]()
									{ return rand_float(min_v, max_v); }), rand_float(min_r, max_r)));
	SpatialDS<Point, DSTag> sd;
	sd.add_points(v.begin(), v.end());
	int col_cnt = 0;
	for (int i = 0; i < all; ++i)
	{
		sd.update();
		for (auto &p : v)
		{
			if (p.color)
				++col_cnt, p.color = 0;
			p.update(spf);
			p.constraint_by_box(Vec::identity() * min_pos, Vec::identity() * max_pos);
		}
	}
	std::cout << name
		 << "\nresult: " << col_cnt
		 << "\ntime " << std::chrono::duration<double>(now() - beg).count() << '\n';
}

int main()
{
	//test<BruteTag>("Brute");
	//test<KDTreeTag>("KDTree");
	//test<PowSplitTreeTag>("PowSplitTree");
	//test<SortSweepTag>("SortSweep");
	test<UniformGridTag>("UniformGrid");
}
