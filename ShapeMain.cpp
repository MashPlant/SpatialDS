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
// Created by mashplant on 18-8-11.
//

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <vector>
#include <sstream>
#include <memory>
#include <iostream>
#include "Point.hpp"
#include "Shape/Shape.hpp"

using Vec = Vec2;
int N = 1500;

Float min_pos = -150, max_pos = 150;
Float min_v = -150, max_v = 150;
Float min_r = 1.25f, max_r = 7.5f;
const float spf = 1.0 / 60.0;

int screen_width = 1200, screen_height = 900;

std::vector<std::unique_ptr<Shape>> ps;

void init()
{
	ps.push_back(std::make_unique<Circle>(200, 200, 100));
	ps.push_back(std::make_unique<Circle>(50, 50, 50));
	ps.push_back(std::make_unique<Triangle>(Vec{-100, 100}, Vec{-120, 80}, Vec{-200, 200}));
	ps.push_back(std::make_unique<Triangle>(Vec{-100, -100}, Vec{0, -100}, Vec{-100, 0}));
	ps.push_back(std::make_unique<Segment>(Vec{50, -50}, Vec{100, -200}));
	ps.push_back(std::make_unique<Segment>(Vec{50, -100}, Vec{200, -200}));
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

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

void tick()
{
	for (auto &p : ps)
		p->collision_on = 0;
	for (int i = 0; i < ps.size(); ++i)
		for (int j = i + 1; j < ps.size(); ++j)
			if (ps[i]->check_collision(*ps[j]))
				ps[i]->collision_on = ps[j]->collision_on = 1;
	glutPostRedisplay();
}

void on_render()
{
	tick();

	glClear(GL_COLOR_BUFFER_BIT);
	for (auto &p : ps)
		p->paint();
	glDisable(GL_LINE_STIPPLE);

	glutSwapBuffers();
}

Vec win_to_gl(int x, int y)
{
	GLint viewport[4];
	GLdouble model[16], project[16];
	GLint real_y;
	GLdouble wx, wy, wz;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, project);
	real_y = viewport[3] - y - 1;
	gluUnProject(x, real_y, 0, model, project, viewport, &wx, &wy, &wz);
	return {static_cast<Float>(wx), static_cast<Float>(wy)};
}

void on_mouse_drag(int x, int y)
{
	static Vec last{-1, 0};
	static int chosen = -1;
	Vec cur = win_to_gl(x, y);
	auto contain = [cur](Shape *p)
	{
		if (auto seg = p->runtime_cast<Segment>())
			return seg->distance(cur) < 5;
		return p->contain(cur);
	};
	if (chosen != -1 && !contain(ps[chosen].get()))
		chosen = -1;
	if (chosen == -1)
		for (int i = 0; i < ps.size() && chosen == -1; ++i)
			chosen = contain(ps[i].get()) ? i : -1;
	else
		ps[chosen]->update(cur - last);
	last = cur;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Shape");
	glutDisplayFunc(on_render);
	glutReshapeFunc(on_size_change);
	glutMotionFunc(on_mouse_drag);
//	glutMo
	init();
	glutMainLoop();
	return 0;
}
