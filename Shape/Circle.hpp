//
// Created by mashplant on 18-8-7.
//

#ifndef SPATIALDS_CIRCLE_HPP
#define SPATIALDS_CIRCLE_HPP

#include "Shape.hpp"

struct Circle : Shape
{
	const static int type = circle;

	Vec2 c;
	Float r;

	Circle(Float x, Float y, Float r) : Shape{circle}, c{x, y}, r{r}
	{}

	Circle(Vec2 c, Float r) : Circle{c.x, c.y, r}
	{}

	static Float radius(const Shape &shape)
	{ return static_cast<const Circle &>(shape).r; }

	static Vec2 position(const Shape &shape)
	{ return static_cast<const Circle &>(shape).c; }

	static bool contain(const Shape &shape, Vec2 point)
	{
		const Circle &circle = static_cast<const Circle &>(shape);
		return circle.c.dis2(point) <= circle.r * circle.r;
	}

	static void update(Shape &shape, Vec2 delta)
	{ static_cast<Circle &>(shape).c += delta; }

	static void destruct(Shape &)
	{}

	static void paint(const Shape&);
};

#endif //SPATIALDS_CIRCLE_HPP
