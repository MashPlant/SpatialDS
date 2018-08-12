//
// Created by mashplant on 18-8-7.
//

#ifndef SPATIALDS_SHAPE_HPP
#define SPATIALDS_SHAPE_HPP

#include "../Common.hpp"
#include "../Vec.hpp"

// I need type_code to implement double dispatch (in check_collision)
// so I give up virtual func completely
// I know it seems (and maybe really is) very awkward

struct Shape
{
	enum
	{
		circle,
		segment,
		triangle,
		undefined,
	};

	const uint8_t type_code;
	uint8_t collision_on;

	constexpr static Float eps = 1e-3;

	using check_collision_t = bool (*)(const Shape &, const Shape &);

	using radius_t = Float (*)(const Shape &);

	using position_t = Vec2 (*)(const Shape &);

	using contain_t = bool (*)(const Shape &, Vec2);

	using update_t = void (*)(Shape &, Vec2);

	using destruct_t = void (*)(Shape &);

	using paint_t = void (*)(const Shape &);

	const static check_collision_t check_collision_table[undefined][undefined];

	const static radius_t radius_table[undefined];

	const static position_t position_table[undefined];

	const static contain_t contain_table[undefined];

	const static update_t update_table[undefined];

	const static destruct_t destruct_table[undefined];

	const static paint_t paint_table[undefined];

	template<typename Derived>
	Derived *runtime_cast()
	{ return type_code == Derived::type ? static_cast<Derived *>(this) : nullptr; }

	template<typename Derived>
	const Derived *runtime_cast() const
	{ return type_code == Derived::type ? static_cast<const Derived *>(this) : nullptr; }

	bool check_collision(const Shape &rhs) const
	{ check_collision_table[type_code][rhs.type_code](*this, rhs); }

	Float radius() const
	{ return radius_table[type_code](*this); }

	Vec2 position() const
	{ return position_table[type_code](*this); }

	bool contain(Vec2 point) const
	{ return contain_table[type_code](*this, point); }

	void update(Vec2 delta)
	{ update_table[type_code](*this, delta); }

	void destruct()
	{ destruct_table[type_code](*this); }

	void paint() const
	{ paint_table[type_code](*this); }
};

#include "Circle.hpp"
#include "Segment.hpp"
#include "Triangle.hpp"
#include "CollisionFunc.hpp"

constexpr inline Shape::check_collision_t Shape::check_collision_table[undefined][undefined] =
		{
				[circle] = {[circle] = collision_circle_circle, [segment] = collision_circle_segment, [triangle] = collision_circle_triangle,},
				[segment] = {[circle] = collision_segment_circle, [segment] = collision_segment_segment, [triangle] = collision_segment_triangle,},
				[triangle] = {[circle] = collision_triangle_circle, [segment] = collision_triangle_segment, [triangle] = collision_triangle_triangle,},
		};

constexpr inline Shape::radius_t Shape::radius_table[undefined] =
		{
				[circle] = Circle::radius,
				[segment] = Segment::radius,
				[triangle] = Triangle::radius,
		};

constexpr inline Shape::position_t Shape::position_table[undefined] =
		{
				[circle] = Circle::position,
				[segment] = Segment::position,
				[triangle] = Triangle::position,
		};

constexpr inline Shape::contain_t Shape::contain_table[undefined] =
		{
				[circle] = Circle::contain,
				[segment] = Segment::contain,
				[triangle] = Triangle::contain,
		};

constexpr inline Shape::update_t Shape::update_table[undefined] =
		{
				[circle] = Circle::update,
				[segment] = Segment::update,
				[triangle] = Triangle::update,
		};

constexpr inline Shape::destruct_t Shape::destruct_table[undefined] =
		{
				[circle] = Circle::destruct,
				[segment] = Segment::destruct,
				[triangle] = Triangle::destruct,
		};

constexpr inline Shape::paint_t Shape::paint_table[undefined] =
		{
				[circle] = Circle::paint,
				[segment] = Segment::paint,
				[triangle] = Triangle::paint,
		};

#endif //SPATIALDS_SHAPE_HPP
