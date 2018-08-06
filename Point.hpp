//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_POINT_HPP
#define SPATIALDS_POINT_HPP

#include "Common.hpp"
#include "Vec.hpp"

template<typename T = void>
struct PointTraits
{
	template<typename Ref>
	static decltype(auto) address(Ref &&ref)
	{ return &std::forward<Ref>(ref); }

	template<typename Ref>
	static decltype(auto) radius(Ref &&ref)
	{ return std::forward<Ref>(ref).radius; }

	template<typename Ref>
	static decltype(auto) get(Ref &&ref, int dim)
	{ return std::forward<Ref>(ref).pos[dim]; }
};

template<typename T>
struct DefaultTypeConfig
{
	using float_type = Float;
	using value_type = T;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type *;
};

struct Point2
{
	Vec2 pos{}, v{};
	Float radius{};
	int color{};

	Point2() = default;

	Point2(Vec2 pos) : pos(pos)
	{}

	Point2(Vec2 pos, Float radius) : pos(pos), radius(radius)
	{}

	Point2(Vec2 pos, Vec2 v, Float radius) : pos(pos), v(v), radius(radius)
	{}

	void update(Float time)
	{ pos += v * time; }

	void constraint_by_box(Vec2 dl, Vec2 ur)
	{
		if (pos.x > ur.x)
			pos.x = ur.x, v.x = -v.x;
		if (pos.y > ur.y)
			pos.y = ur.y, v.y = -v.y;
		if (pos.x < dl.x)
			pos.x = dl.x, v.x = -v.x;
		if (pos.y < dl.y)
			pos.y = dl.y, v.y = -v.y;
	}
};

template<>
struct PointTraits<Point2> : PointTraits<>, DefaultTypeConfig<Point2>
{
	const static int dim = 2;

	using PointTraits<>::get;

	template<int Dim>
	static Float get(const_reference p)
	{
		static_assert(Dim == 0 || Dim == 1);
		if constexpr (Dim == 0)
			return p.pos.x;
		else
			return p.pos.y;
	}

	static void check_collision(reference lhs, reference rhs)
	{
		if (lhs.pos.dis2(rhs.pos) <= (lhs.radius + rhs.radius) * (lhs.radius + rhs.radius))
			lhs.color = rhs.color = 1;
	}
};

struct Point3
{
	Vec3 pos{}, v{};
	Float radius{};
	int color{};

	Point3() = default;

	Point3(Vec3 pos) : pos(pos)
	{}

	Point3(Vec3 pos, Float radius) : pos(pos), radius(radius)
	{}

	Point3(Vec3 pos, Vec3 v, Float radius) : pos(pos), v(v), radius(radius)
	{}

	void update(Float time)
	{ pos += v * time; }

	void constraint_by_box(Vec3 min, Vec3 max)
	{
		if (pos.x > max.x)
			pos.x = max.x, v.x = -v.x;
		if (pos.y > max.y)
			pos.y = max.y, v.y = -v.y;
		if (pos.z > max.z)
			pos.z = max.z, v.z = -v.z;
		if (pos.x < min.x)
			pos.x = min.x, v.x = -v.x;
		if (pos.y < min.y)
			pos.y = min.y, v.y = -v.y;
		if (pos.z < min.z)
			pos.z = min.z, v.z = -v.z;
	}
};

template<>
struct PointTraits<Point3> : PointTraits<>, DefaultTypeConfig<Point3>
{
	const static int dim = 3;

	using PointTraits<>::get;

	template<int Dim>
	static Float get(const_reference p)
	{
		static_assert(Dim == 0 || Dim == 1 || Dim == 2);
		if constexpr (Dim == 0)
			return p.pos.x;
		else if constexpr (Dim == 1)
			return p.pos.y;
		else
			return p.pos.z;
	}

	static void check_collision(reference lhs, reference rhs)
	{
		if (lhs.pos.dis2(rhs.pos) <= (lhs.radius + rhs.radius) * (lhs.radius + rhs.radius))
			lhs.color = rhs.color = 1;
	}
};

#endif //SPATIALDS_POINT_HPP
