#include "Shape.hpp"
#include <iostream>

using namespace std;

int main()
{
	auto in = Segment::intersection({{50, -50}, {100, -200}}, {{50, -100}, {200, -200}});
	cout << in.x << ' ' << in.y;
}
