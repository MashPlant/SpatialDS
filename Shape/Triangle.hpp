//
// Created by mashplant on 18-8-8.
//

#ifndef SPATIALDS_TRIANGLE_HPP
#define SPATIALDS_TRIANGLE_HPP

#include "Shape.hpp"

struct Triangle : Shape
{
	Vec2 p1, p2, p3;
	Vec2 c;
	Float r; // save the center & radius of circumscribed circle

	Triangle(Vec2 p1, Vec2 p2, Vec2 p3) : Shape{triangle}, p1{p1}, p2{p2}, p3{p3}
	{
		Float Bx = p2.x - p1.x, By = p2.y - p1.y;
		Float Cx = p3.x - p1.x, Cy = p3.y - p1.y;
		Float D = 2 * (Bx * Cy - By * Cx);
		c = {(Cy * (Bx * Bx + By * By) - By * (Cx * Cx + Cy * Cy)) / D + p1.x,
			 (Bx * (Cx * Cx + Cy * Cy) - Cx * (Bx * Bx + By * By)) / D + p1.y};
		r = c.dis(p1);
	}

	static Float radius(const Shape &shape)
	{ return static_cast<const Triangle &>(shape).r; }

	static Vec2 position(const Shape &shape)
	{ return static_cast<const Triangle &>(shape).c; }

	static bool contain(const Shape &shape, Vec2 point)
	{
		const Triangle &tr = static_cast<const Triangle &>(shape);
		Float A = 0.5 * (-tr.p2.y * tr.p3.x + tr.p1.y * (-tr.p2.x + tr.p3.x) + tr.p1.x * (tr.p2.y - tr.p3.y) +
						 tr.p2.x * tr.p3.y);
		int sgn = A < 0 ? -1 : 1;
		Float s = (tr.p1.y * tr.p3.x - tr.p1.x * tr.p3.y + (tr.p3.y - tr.p1.y) * point.x +
				   (tr.p1.x - tr.p3.x) * point.y) * sgn;
		Float t = (tr.p1.x * tr.p2.y - tr.p1.y * tr.p2.x + (tr.p1.y - tr.p2.y) * point.x +
				   (tr.p2.x - tr.p1.x) * point.y) * sgn;
		return s >= 0 && t >= 0 && (s + t) <= 2 * A * sgn;
	}

	static void update(Shape &shape, Vec2 delta)
	{
		Triangle &t = static_cast<Triangle &>(shape);
		t.p1 += delta, t.p2 += delta, t.p3 += delta;
		t.c += delta;
	}

	static void destruct(Shape &)
	{}
};

#endif //SPATIALDS_TRIANGLE_HPP
