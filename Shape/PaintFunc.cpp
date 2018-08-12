//
// Created by mashplant on 18-8-11.
//

#include "Shape.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void Circle::paint(const Shape &shape)
{
	const int n = 36;
	const Float angle = 2 * acos(-1) / n;
	const Circle &c = static_cast<const Circle &>(shape);
	glBegin(GL_POLYGON);
	c.collision_on ? glColor3f(1.0f, 0.0f, 0.0f) : glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < n; ++i)
		glVertex2f(c.c.x + cos(angle * i) * c.r, c.c.y + sin(angle * i) * c.r);
	glEnd();
}

void Segment::paint(const Shape &shape)
{
	const Segment &s = static_cast<const Segment &>(shape);
	s.collision_on ? glColor3f(1.0f, 0.0f, 0.0f) : glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(s.p1.x, s.p1.y);
	glVertex2f(s.p2.x, s.p2.y);
	glEnd();
}

void Triangle::paint(const Shape &shape)
{
	const Triangle &t = static_cast<const Triangle &>(shape);
	glBegin(GL_POLYGON);
	t.collision_on ? glColor3f(1.0f, 0.0f, 0.0f) : glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(t.p1.x, t.p1.y);
	glVertex2f(t.p2.x, t.p2.y);
	glVertex2f(t.p3.x, t.p3.y);
	glEnd();
}

