//
// Created by mashplant on 18-8-7.
//

#ifndef SPATIALDS_SEGMENT_HPP
#define SPATIALDS_SEGMENT_HPP

#include "Shape.hpp"

struct Segment : Shape
{
	Vec2 p1, p2;

	static Vec2 intersection(const Segment &s1, const Segment &s2)
	{
		Vec2 l1 = s1.p2 - s1.p1, l2 = s2.p2 - s2.p1;
		return s1.p1 + l1 * (Vec2::cross(l2, s2.p1 - s1.p1) / Vec2::cross(l2, l1));
	}

	Float distance(Vec2 point) const
	{
		Vec2 d1 = point - p1, d2 = point - p2, l = p2 - p1;
		if (Vec2::dot(d1, l) < 0) // angle CP1P2 is obtuse
			return d1.len();
		else if (Vec2::dot(d2, l) > 0) // angle CP2P1 is obtuse
			return d2.len();
		else // distance dot to line
			return std::abs(Vec2::cross(d1, l)) / l.len();
	}

	Segment(Float k, Float b, Float x1, Float x2)
			: Shape{segment}, p1{x1, x1 * k + b}, p2{x2, x2 * k + b}
	{}

	Segment(Vec2 p1, Vec2 p2) : Shape{segment}, p1{p1}, p2{p2}
	{}

	static Float radius(const Shape &shape)
	{
		const Segment &segment = static_cast<const Segment &>(shape);
		return segment.p1.dis(segment.p2);
	}

	static Vec2 position(const Shape &shape)
	{
		const Segment &segment = static_cast<const Segment &>(shape);
		return 0.5 * (segment.p1 + segment.p2);
	}

	static bool contain(const Shape &shape, Vec2 point)
	{
		const Segment &segment = static_cast<const Segment &>(shape);
		Vec2 d1 = segment.p1 - point, d2 = segment.p2 - point;
		return std::abs(Vec2::cross(d1, d2)) <= eps && Vec2::dot(d1, d2) <= 0;
	}

	static void update(Shape &shape, Vec2 delta)
	{
		Segment &segment = static_cast< Segment &>(shape);
		segment.p1 += delta, segment.p2 += delta;
	}

	static void destruct(Shape &)
	{}
};

#endif //SPATIALDS_SEGMENT_HPP
