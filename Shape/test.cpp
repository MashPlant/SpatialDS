#include "Shape.hpp"
#include <iostream>

using namespace std;

struct A
{
	const static int f;
};
const int A::f = rand();

int main()
{
//	Shape *tr = new Circle(-0.7, -0.7, 1);

	Triangle *tr = new Triangle({-0.5, 0}, {0.5, 0}, {0, 1});
	cout << tr->c.x << ' ' << tr->c.y << ' ' << tr->r << ' ';
//	Shape *tr2 = new Triangle({}, {}, {});
//	Shape *c = new Circle({0, 0}, 1);
//	for (Float y = 1.5; y >= -0.5; y -= 0.1, cout << endl)
//		for (Float x = -0.5; x <= 1.5; x += 0.1)
//			cout << (cir->contain({x, y}) || tri->contain({x, y}));
//	for (int i = 0; i < 20; ++i)
//	{
//		c->update(*c, {0.1, 0});
//		cout << tr->check_collision(*tr, *c);
//	}

//	for (Float y = 2; y >= -0.5; y -= 0.1, cout << endl)
//		for (Float x = -0.5; x <= 2; x += 0.1)
//			printf("%d ", (int) (tr->contain(*tr, {x, y})));
}