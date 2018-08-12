//
// Created b.y mashplant on 18-8-7.
//

#ifndef SPATIALDS_COLLISIONFUNC_HPP
#define SPATIALDS_COLLISIONFUNC_HPP

#include "Shape.hpp"

#define P(x) (printf("%s=%d\n",#x,x), x)

inline bool collision_circle_circle(const Shape &lhs, const Shape &rhs)
{
	const Circle &c1 = static_cast<const Circle &>(lhs),
			&c2 = static_cast<const Circle &>(rhs);
	return c1.c.dis2(c2.c) <= (c1.r + c2.r) * (c1.r + c2.r);
}

inline bool collision_circle_segment(const Shape &lhs, const Shape &rhs)
{
	const Circle &c = static_cast<const Circle &>(lhs);
	const Segment &s = static_cast<const Segment &>(rhs);
	return s.distance(c.c) <= c.r;
}

inline bool collision_circle_triangle(const Shape &lhs, const Shape &rhs)
{
	const Circle &c = static_cast<const Circle &>(lhs);
	const Triangle &t = static_cast<const Triangle &>(rhs);
	return Triangle::contain(t, c.c) // the center of circle is in triangle
		   || collision_circle_segment(c, Segment{t.p1, t.p2}) // or any of the edge intersects with the circle
		   || collision_circle_segment(c, Segment{t.p2, t.p3})
		   || collision_circle_segment(c, Segment{t.p3, t.p1});
}

inline bool collision_segment_circle(const Shape &lhs, const Shape &rhs)
{ return collision_circle_segment(rhs, lhs); }

inline bool collision_segment_segment(const Shape &lhs, const Shape &rhs)
{
	const Segment &s1 = static_cast<const Segment &>(lhs),
			&s2 = static_cast<const Segment &>(rhs);
	if (std::abs(Vec2::cross(s1.p2 - s1.p1, s2.p2 - s2.p1)) <= Shape::eps) // parallel
		return P(Segment::contain(s1, s2.p1) || Segment::contain(s1, s2.p2)
			   || Segment::contain(s2, s1.p1) || Segment::contain(s2, s1.p2));
	Vec2 intersect_pos = Segment::intersection(s1, s2); // not parallel, find intersection
	return P(Segment::contain(s1, intersect_pos) && Segment::contain(s2, intersect_pos));
}

inline bool collision_segment_triangle(const Shape &lhs, const Shape &rhs)
{
	const Segment &s = static_cast<const Segment &>(lhs);
	const Triangle &t = static_cast<const Triangle &>(rhs);
	return Triangle::contain(t, s.p1) // one of the end is in triangle
		   || Triangle::contain(t, s.p2)
		   || collision_segment_segment(s, Segment{t.p1, t.p2}) // or any of the edge intersects with segment
		   || collision_segment_segment(s, Segment{t.p2, t.p3})
		   || collision_segment_segment(s, Segment{t.p3, t.p1});
}

inline bool collision_triangle_circle(const Shape &lhs, const Shape &rhs)
{ return collision_circle_triangle(rhs, lhs); }

inline bool collision_triangle_segment(const Shape &lhs, const Shape &rhs)
{ return collision_segment_triangle(rhs, lhs); }

inline bool collision_triangle_triangle(const Shape &lhs, const Shape &rhs)
{
	auto cross = [](const Triangle &t, Vec2 b, Vec2 c, Float normal)
	{
		Float cy_by = c.y - b.y, cx_bx = c.x - b.x;
		return ((t.p1.x - b.x) * cy_by - (t.p1.y - b.y) * cx_bx) * normal >= 0
			   && ((t.p2.x - b.x) * cy_by - (t.p2.y - b.y) * cx_bx) * normal >= 0
			   && ((t.p3.x - b.x) * cy_by - (t.p3.y - b.y) * cx_bx) * normal >= 0;
	};
	const Triangle &t1 = static_cast<const Triangle &>(lhs),
			&t2 = static_cast<const Triangle &>(rhs);
	Float normal1 = (t1.p2.x - t1.p1.x) * (t1.p3.y - t1.p1.y) -
					(t1.p2.y - t1.p1.y) * (t1.p3.x - t1.p1.x),
			normal2 = (t2.p2.x - t2.p1.x) * (t2.p3.y - t2.p1.y) -
					  (t2.p2.y - t2.p1.y) * (t2.p3.x - t2.p1.x);
	return !(cross(t2, t1.p1, t1.p2, normal1) ||
			 cross(t2, t1.p2, t1.p3, normal1) ||
			 cross(t2, t1.p3, t1.p1, normal1) ||
			 cross(t1, t2.p1, t2.p2, normal2) ||
			 cross(t1, t2.p2, t2.p3, normal2) ||
			 cross(t1, t2.p3, t2.p1, normal2));
}

#endif //SPATIALDS_COLLISIONFUNC_HPP
