/*
 *  Created by radix on 05/04/08.
 *  Copyright Mykola Konyk, <mykola@konyk.org>, 2008.
 *
 *  This code is under Microsoft Reciprocal License (Ms-RL)
 *  Please see http://www.opensource.org/licenses/ms-rl.html
 *
 *  Important points about the license (from Ms-RL):
 *
 *  [A] For any file you distribute that contains code from the software (in source code or binary format), you must provide
 *  recipients the source code to that file along with a copy of this license, which license will govern that file.
 *  You may license other files that are entirely your own work and do not contain code from the software under any terms
 *  you choose.
 *
 *  [B] No Trademark License- This license does not grant you rights to use any contributors' name, logo, or trademarks.
 *
 *  [C] If you bring a patent claim against any contributor over patents that you claim are infringed by the software, your
 *  patent license from such contributor to the software ends automatically.
 *
 *  [D] If you distribute any portion of the software, you must retain all copyright, patent, trademark, and attribution notices
 *  that are present in the software.
 *
 *  [E] If you distribute any portion of the software in source code form, you may do so only under this license by including a
 *  complete copy of this license with your distribution. If you distribute any portion of the software in compiled or object
 *  code form, you may only do so under a license that complies with this license.
 *
 *  [F] The software is licensed "as-is." You bear the risk of using it. The contributors give no express warranties, guarantees
 *  or conditions. You may have additional consumer rights under your local laws which this license cannot change. To the extent
 *  permitted under your local laws, the contributors exclude the implied warranties of merchantability, fitness for a particular
 *  purpose and non-infringement.
 *
 */

//
// Created by mashplant on 18-8-3.
//

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <vector>
#include <variant>
#include <sstream>
#include "Point.hpp"
#include "SpatialDS.hpp"
#include "SpatialDSImpl/Brute.hpp"
#include "SpatialDSImpl/KDTree.hpp"
#include "SpatialDSImpl/PowSplitTree.hpp"
#include "SpatialDSImpl/SortSweep.hpp"
#include "SpatialDSImpl/UniformGrid.hpp"
#include "SpatialDSImpl/ControlGroup.hpp"

#include "CmdLine/CmdLine.hpp"

using Point = Point3;
using Vec = decltype(Point{}.pos);

std::vector<Point> point_entity;
using PolymorphicDS = std::variant
		<
				SpatialDS<Point, BruteTag>,
				SpatialDS<Point, KDTreeTag>,
				SpatialDS<Point, PowSplitTreeTag>,
				SpatialDS<Point, SortSweepTag>,
				SpatialDS<Point, UniformGridTag>,
				SpatialDS<Point, ControlGroupTag>
		>;
PolymorphicDS ds = SpatialDS<Point, BruteTag>{};
int ds_type = 0;

int N = 1500, col_cnt = 0;

Float min_pos = -150, max_pos = 150;
Float min_v = -150, max_v = 150;
Float min_r = 1.25f, max_r = 7.5f;
const float spf = 1.0 / 60.0;

int screen_width = 1200, screen_height = 900;
Float rotate_x, rotate_y;

Float rand_float(Float l, Float r)
{ return l + (r - l) * (static_cast<Float>(rand()) / RAND_MAX); }

void on_size_change(GLsizei w, GLsizei h)
{
	screen_width = w, screen_height = h;
	GLfloat range = max_pos - min_pos;
	if (!h)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-range, range, -range * h / w, range * h / w, -range, range);
	else
		glOrtho(-range * w / h, range * w / h, -range, range, -range, range);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	srand(19260817);
	using trait = PointTraits<Point>;
	for (int _ = 0; _ < N; ++_)
		point_entity.push_back(Point(Vec::rand([=]()
											   { return rand_float(min_pos, max_pos); }),
									 Vec::rand([=]()
											   { return rand_float(min_v, max_v); }), rand_float(min_r, max_r)));
	std::visit([](auto &x)
			   { x.add_points(point_entity.begin(), point_entity.end()); }, ds);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void tick()
{
	col_cnt = 0;
	for (auto &p : point_entity)
	{
		if (p.color)
			++col_cnt, p.color = 0;
		p.update(spf);
		p.constraint_by_box(Vec::identity() * min_pos, Vec::identity() * max_pos);
	}
	std::visit([](auto &x)
			   { x.update(); }, ds);
	glutPostRedisplay();
}

void render_info();

void on_render()
{
	tick();

	glClear(GL_COLOR_BUFFER_BIT);

	render_info();

	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f), rotate_x = 0;
	glRotatef(rotate_y, 0.0f, 1.0f, 0.0f), rotate_y = 0;

	for (auto &p : point_entity)
	{
		glPushMatrix();
		glTranslatef(p.pos.x, p.pos.y, p.pos.z);
		if (p.color)
			glColor3f(1.0f, 0.0f, 0.0f);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidCube(p.radius);
		glPopMatrix();
	}
	// box
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0x0F0F);

	glBegin(GL_LINE_STRIP);

	glVertex3f(min_pos, max_pos, min_pos);
	glVertex3f(max_pos, max_pos, min_pos);
	glVertex3f(max_pos, max_pos, max_pos);
	glVertex3f(min_pos, max_pos, max_pos);
	glVertex3f(min_pos, max_pos, min_pos);

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex3f(min_pos, min_pos, min_pos);
	glVertex3f(max_pos, min_pos, min_pos);
	glVertex3f(max_pos, min_pos, max_pos);
	glVertex3f(min_pos, min_pos, max_pos);
	glVertex3f(min_pos, min_pos, min_pos);

	glEnd();

	glBegin(GL_LINES);

	glVertex3f(min_pos, max_pos, max_pos);
	glVertex3f(min_pos, min_pos, max_pos);

	glVertex3f(min_pos, max_pos, min_pos);
	glVertex3f(min_pos, min_pos, min_pos);

	glVertex3f(max_pos, max_pos, min_pos);
	glVertex3f(max_pos, min_pos, min_pos);

	glVertex3f(max_pos, max_pos, max_pos);
	glVertex3f(max_pos, min_pos, max_pos);

	glEnd();

	glDisable(GL_LINE_STIPPLE);

	glutSwapBuffers();
}

void on_mouse_drag(int x, int y)
{
	static int last_x = -1, last_y;
	if (last_x != -1)
		rotate_x = (last_x - x) * 0.5, rotate_y = (last_y - y) * 0.5;
	last_x = x, last_y = y;
}

void on_key_down(unsigned char ch_key, int, int)
{
	static PolymorphicDS (*(key_to_ds[]))() =
			{
					[0] = []() -> PolymorphicDS
					{ return SpatialDS<Point, BruteTag>{}; },
					[1] = []() -> PolymorphicDS
					{ return SpatialDS<Point, KDTreeTag>{}; },
					[2] = []() -> PolymorphicDS
					{ return SpatialDS<Point, PowSplitTreeTag>{}; },
					[3] = []() -> PolymorphicDS
					{ return SpatialDS<Point, SortSweepTag>{}; },
					[4] = []() -> PolymorphicDS
					{ return SpatialDS<Point, UniformGridTag>{}; },
					[5] = []() -> PolymorphicDS
					{ return SpatialDS<Point, ControlGroupTag>{}; }
			};
	int key = ch_key - '0';
	if (key >= 0 && key <= sizeof(key_to_ds) / sizeof(key_to_ds[0]))
		ds = key_to_ds[key](), ds_type = key;
	std::visit([](auto &x)
			   { x.add_points(point_entity.begin(), point_entity.end()); }, ds);
}

static void print_text(const std::string &msg, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, screen_width, 0, screen_height);
	glScalef(1, -1, 1);
	glTranslatef(0, -screen_height, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 0.0f);

	glRasterPos2f(x, y);

	for (int i = 0; i < msg.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void render_info()
{
	static std::string names[] = {[0]="Brute", [1]="KDTree", [2] ="PowSplitTree", [3]="SortSweep", [4]="UniformGrid", [5] = "ControlGroup"};
	static int last_time = 0, frame_cnt = 0;
	static float fps = 0, sum_col_cnt = 0, ave_col_cnt = 0;
	++frame_cnt, sum_col_cnt += col_cnt;
	int time = glutGet(GLUT_ELAPSED_TIME);
	{
		std::stringstream ss;
		for (auto &name : names)
			ss << '[' << &name - names << ']' << name << " ";
		print_text(ss.str(), 20, 25);
	}
	print_text("current spatial ds: " + names[ds_type], 20, 50);
	if (time - last_time > 1000)
	{
		fps = frame_cnt * 1000.0f / (time - last_time);
		ave_col_cnt = sum_col_cnt / frame_cnt;
		last_time = time, frame_cnt = 0, sum_col_cnt = 0;
	}
	print_text("fps: " + std::to_string(fps), 20, 75);
	print_text("object count: " + std::to_string(N), 20, 100);
	print_text("collision count: " + std::to_string(col_cnt), 20, 125);
	print_text("average collision count: " + std::to_string(ave_col_cnt), 20, 150);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("SpatialDS");
	glutDisplayFunc(on_render);
	glutReshapeFunc(on_size_change);
	glutMotionFunc(on_mouse_drag);
	glutKeyboardFunc(on_key_down);

	cmdline::parser parser;
	parser.add<int>("number", 'n', "number of objects", false, 1000,
					cmdline::range(1, std::numeric_limits<int>::max()));
	parser.add<Float>("max_pos", '\0', "max pos of objects", false, 150, cmdline::range(50, 200));
	parser.add<Float>("min_pos", '\0', "min pos of objects", false, -150, cmdline::range(-200, -50));
	parser.add<Float>("max_v", '\0', "max velocity of objects", false, 150, cmdline::range(50, 200));
	parser.add<Float>("min_v", '\0', "min velocity of objects", false, -150, cmdline::range(-200, -50));
	parser.add<Float>("max_r", '\0', "max radius of objects", false, 7.5, cmdline::range(5, 10));
	parser.add<Float>("min_r", '\0', "min radius of objects", false, 1.5, cmdline::range(0, 5));
	parser.parse_check(argc, argv);
	N = parser.get<int>("number");
	max_pos = parser.get<Float>("max_pos");
	min_pos = parser.get<Float>("min_pos");
	max_v = parser.get<Float>("max_v");
	min_v = parser.get<Float>("min_v");
	max_r = parser.get<Float>("max_r");
	min_r = parser.get<Float>("min_r");

	init();
	glutMainLoop();
	return 0;
}
