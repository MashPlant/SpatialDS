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
		circle = 0,
		segment = 1,
		triangle = 2,
		undefined = 3,
	} const type_code = undefined;

	constexpr static Float eps = 1e-5;

	using check_collision_t = bool (*)(const Shape &, const Shape &);

	using radius_t = Float (*)(const Shape &);

	using position_t = Vec2 (*)(const Shape &);

	using contain_t = bool (*)(const Shape &, Vec2);

	using update_t = void (*)(Shape &, Vec2);

	using destruct_t = void (*)(Shape &);

	const static check_collision_t check_collision_table[undefined][undefined];

	const static radius_t radius_table[undefined];

	const static position_t position_table[undefined];

	const static contain_t contain_table[undefined];

	const static update_t update_table[undefined];

	const static destruct_t destruct_table[undefined];

	static bool check_collision(const Shape &lhs, const Shape &rhs);

	static Float radius(const Shape &shape);

	static Vec2 position(const Shape &shape);

	static bool contain(const Shape &shape, Vec2 point);

	static void update(Shape &shape, Vec2 delta);

	static void destruct(Shape &shape);
};

#include "Circle.hpp"
#include "Segment.hpp"
#include "Triangle.hpp"
#include "CollisionFunc.hpp"

constexpr Shape::check_collision_t Shape::check_collision_table[undefined][undefined] =
		{
				[circle] = {[circle] = collision_circle_circle, [segment] = collision_circle_segment, [triangle] = collision_circle_triangle,},
				[segment] = {[circle] = collision_segment_circle, [segment] = collision_segment_segment, [triangle] = collision_segment_triangle,},
				[triangle] = {[circle] = collision_triangle_circle, [segment] = collision_triangle_segment, [triangle] = collision_triangle_triangle,},
		};

constexpr Shape::radius_t Shape::radius_table[undefined] =
		{
				[circle] = Circle::radius,
				[segment] = Segment::radius,
				[triangle] = Triangle::radius,
		};

constexpr Shape::position_t Shape::position_table[undefined] =
		{
				[circle] = Circle::position,
				[segment] = Segment::position,
				[triangle] = Triangle::position,
		};

constexpr Shape::contain_t Shape::contain_table[undefined] =
		{
				[circle] = Circle::contain,
				[segment] = Segment::contain,
				[triangle] = Triangle::contain,
		};

constexpr Shape::update_t Shape::update_table[undefined] =
		{
				[circle] = Circle::update,
				[segment] = Segment::update,
				[triangle] = Triangle::update,
		};

constexpr Shape::destruct_t Shape::destruct_table[undefined] =
		{
				[circle] = Circle::destruct,
				[segment] = Segment::destruct,
				[triangle] = Triangle::destruct,
		};

bool Shape::check_collision(const Shape &lhs, const Shape &rhs)
{ return check_collision_table[lhs.type_code][rhs.type_code](lhs, rhs); }

Float Shape::radius(const Shape &shape)
{ return radius_table[shape.type_code](shape); }

Vec2 Shape::position(const Shape &shape)
{ return position_table[shape.type_code](shape); }

bool Shape::contain(const Shape &shape, Vec2 point)
{ return contain_table[shape.type_code](shape, point); }

void Shape::update(Shape &shape, Vec2 delta)
{ return update_table[shape.type_code](shape, delta); }

void Shape::destruct(Shape &shape)
{ return destruct_table[shape.type_code](shape); }

#endif //SPATIALDS_SHAPE_HPP
