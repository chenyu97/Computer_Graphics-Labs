#include <iostream>
#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
using namespace std;

#define InitialWidth 320
#define InitialHeight 320
#define LINE 1
#define LINE_STATE1 101
#define LINE_STATE2 102
#define LINE_STATE3 103
#define CIRCLE 2
#define CIRCLE_STATE1 201
#define CIRCLE_STATE2 202
#define CIRCLE_STATE3 203
#define CIRCLE_STATE4 204
#define CIRCLE_STATE5 205
#define CIRCLE_STATE6 206
#define POLYGON 3
#define POLYGON_STATE1 301
#define POLYGON_STATE2 302
#define POLYGON_STATE3 303
#define POLYGON_STATE4 304
#define POLYGON_STATE5 305
#define ELLIPSE 4
#define ELLIPSE_STATE1 401
#define ELLIPSE_STATE2 402
#define ELLIPSE_STATE3 403
#define ELLIPSE_STATE4 404
#define ELLIPSE_STATE5 405
#define ELLIPSE_STATE6 406

int system_state = 0;
float CurrentWidth = InitialWidth;
float CurrentHeight = InitialHeight;
int left_button_down = 0;
int left_button_up = 0;
int edit_polygon_point = -1;
int isCircleEdit = 0;
int isEllipseEdit = 0;

struct Line
{
	int x_1;
	int y_1;
	int x_2;
	int y_2;
};

struct Ellipse
{
	int x;
	int y;
	int x_half_length;
	int y_half_length;
};

vector<Line> lines;
vector<vector<Line>> polygons;
vector<Ellipse> ellipses;

Line circleBounds[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };  //to surround the current circle
Line ellipseBounds[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };  //to surround the current ellipse

void drawLines(int x_1, int y_1, int x_2, int y_2)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);

	int dx, dy;
	dx = x_2 - x_1;
	dy = y_2 - y_1;

	if (x_1 == x_2)
	{	//one special case
		int min = (y_1 <= y_2) ? y_1 : y_2;
		int max = y_1 + y_2 - min;
		for (int i = min; i <= max; i++)
		{
			glVertex2i(x_1, i);
		}
	}
	else if (y_1 == y_2)
	{	//the other special case
		int min = (x_1 <= x_2) ? x_1 : x_2;
		int max = x_1 + x_2 - min;
		for (int i = min; i <= max; i++)
		{
			glVertex2i(i, y_1);
		}
	}
	else if (abs(dx) >= abs(dy))
	{	//abs(dx) >= abs(dy)
		if ((dx > 0) == (dy > 0))
		{	//k > 0
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_1;
				x_min = x_2;
				y_min = y_2;
			}
			else
			{
				x_max = x_2;
				y_max = y_2;
				x_min = x_1;
				y_min = y_1;
			}
			int ddx = x_max - x_min;
			int ddy = y_max - y_min;
			int x = x_min;
			int y = y_min;
			int p = 2 * ddy - ddx;
			for (; x <= x_max; x++)
			{
				glVertex2i(x, y);
				if (p >= 0)
				{
					y++;
					p += 2 * (ddy - ddx);
				}
				else
				{
					p += 2 * ddy;
				}
			}
		}
		else
		{	//k <0
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_2;
				x_min = x_2;
				y_min = y_1;
			}
			else
			{
				x_max = x_2;
				y_max = y_1;
				x_min = x_1;
				y_min = y_2;
			}
			int ddx = x_max - x_min;
			int ddy = y_max - y_min;
			int x = x_max;
			int y = y_min;
			int p = 2 * ddy - ddx;
			for (; x >= x_min; x--)
			{
				glVertex2i(x, y);
				if (p >= 0)
				{
					y++;
					p += 2 * (ddy - ddx);
				}
				else
				{
					p += 2 * ddy;
				}
			}
		}
	}
	else if (abs(dx) < abs(dy))
	{	//abs(dx) < abs(dy)
		if ((dx > 0) == (dy > 0))
		{
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_1;
				x_min = x_2;
				y_min = y_2;

				cout << '(' << x_min << ',' << y_min << ") ,(" << x_max << ',' << y_max << ") = =" << endl;

			}
			else
			{
				x_max = x_2;
				y_max = y_2;
				x_min = x_1;
				y_min = y_1;
			}
			int ddx = x_max - x_min;
			int ddy = y_max - y_min;
			int x = x_min;
			int y = y_min;
			int p = 2 * ddx - ddy;
			for (; y <= y_max; y++)
			{
				glVertex2i(x, y);
				if (p >= 0)
				{
					x++;
					p += 2 * (ddx - ddy);
				}
				else
				{
					p += 2 * ddx;
				}
			}
		}
		else
		{
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_2;
				x_min = x_2;
				y_min = y_1;
			}
			else
			{
				x_max = x_2;
				y_max = y_1;
				x_min = x_1;
				y_min = y_2;
			}
			int ddx = x_max - x_min;
			int ddy = y_max - y_min;
			int x = x_max;
			int y = y_min;
			int p = 2 * ddx - ddy;
			for (; y <= y_max; y++)
			{
				glVertex2i(x, y);
				if (p >= 0)
				{
					x--;
					p += 2 * (ddx - ddy);
				}
				else
				{
					p += 2 * ddx;
				}
			}
		}
	}
	glEnd();
	return;
}

void drawEllipse(Ellipse e)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);

	//consider x = 0, y = 0; afterwards we move all the points
	//this is part 1:
	int p0 = e.y_half_length * e.y_half_length - e.x_half_length * e.x_half_length * e.y_half_length + e.x_half_length * e.x_half_length / 4;
	int x = 0;
	int y = e.y_half_length;
	int last_x = x;
	int last_y = y;
	int p = p0;
	while (e.y_half_length * e.y_half_length * x - e.x_half_length * e.x_half_length * y < 0)
	{
		//draw
		glVertex2i(x + e.x, y + e.y);
		glVertex2i((-1) * x + e.x, (-1) * y + e.y);
		glVertex2i((-1) * x + e.x, y + e.y);
		glVertex2i(x + e.x, (-1) * y + e.y);
		if (p < 0)
		{
			p = p + 2 * e.y_half_length * e.y_half_length * x + 3 * e.y_half_length * e.y_half_length;
			last_x = x;
			last_y = y;
			x = x + 1;
			y = y;
		}
		else if (p >= 0)
		{
			p = p + 2 * e.y_half_length * e.y_half_length * x + 3 * e.y_half_length * e.y_half_length - 2 * e.x_half_length * e.x_half_length * y + 2 * e.x_half_length * e.x_half_length;
			last_x = x;
			last_y = y;
			x = x + 1;
			y = y - 1;
		}
	}
	//this is part 2:
	x = last_x;
	y = last_y;
	p0 =  e.y_half_length * e.y_half_length * (x + 1 / 2) * (x + 1 / 2)  + e.x_half_length * e.x_half_length * (y - 1) * (y - 1) - e.x_half_length * e.x_half_length * e.y_half_length * e.y_half_length;
	p = p0;
	while (x <= e.x_half_length && y >= 0)
	{
		//draw
		glVertex2i(x + e.x, y + e.y);
		glVertex2i((-1) * x + e.x, (-1) * y + e.y);
		glVertex2i((-1) * x + e.x, y + e.y);
		glVertex2i(x + e.x, (-1) * y + e.y);
		if (p > 0)
		{
			p = p - 2 * e.x_half_length * e.x_half_length * y + 3 * e.x_half_length * e.x_half_length;
			x = x;
			y = y - 1;
		}
		else if (p <= 0)
		{
			p = p - 2 * e.x_half_length * e.x_half_length * y + 3 * e.x_half_length * e.x_half_length + 2 * e.y_half_length * e.y_half_length * x + 2 * e.y_half_length * e.y_half_length;
			x = x + 1;
			y = y - 1;
		}
	}
	glEnd();
	return;
}

void InitEnvironment()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH) * 2, glutGet(GLUT_SCREEN_HEIGHT) * 2);
	gluOrtho2D(0, glutGet(GLUT_SCREEN_WIDTH) * 2, 0, glutGet(GLUT_SCREEN_HEIGHT) * 2);

	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w, int h) {

	CurrentWidth = w;
	CurrentHeight = h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH) * 2, glutGet(GLUT_SCREEN_HEIGHT) * 2);

	gluOrtho2D(0, glutGet(GLUT_SCREEN_WIDTH) * 2, 0, glutGet(GLUT_SCREEN_HEIGHT) * 2);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw lines
	for (int i = 0; i < lines.size(); i++)
	{
		cout << '(' << lines[i].x_1 << ',' << lines[i].y_1 << ") ,(" << lines[i].x_2 << ',' << lines[i].y_2 << ')' << endl;
		drawLines(lines[i].x_1, lines[i].y_1, lines[i].x_2, lines[i].y_2);
	}

	//draw polygons
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = 0; j < polygons[i].size(); j++)
			drawLines(polygons[i][j].x_1, polygons[i][j].y_1, polygons[i][j].x_2, polygons[i][j].y_2);
	}

	//draw circleBounds
	if (isCircleEdit)
	{
		for (int i = 0; i < 4; i++)
		{
			drawLines(circleBounds[i].x_1, circleBounds[i].y_1, circleBounds[i].x_2, circleBounds[i].y_2);
		}
	}

	//draw ellipseBounds
	if (isEllipseEdit)
	{
		for (int i = 0; i < 4; i++)
		{
			drawLines(ellipseBounds[i].x_1, ellipseBounds[i].y_1, ellipseBounds[i].x_2, ellipseBounds[i].y_2);
		}
	}
	
	//draw ellipses
	for (int i = 0; i < ellipses.size(); i++)
	{
		drawEllipse(ellipses[i]);
	}

	glFlush();
}

void mouseButton(int button, int state, int x, int y)
{
	switch (system_state){
	case LINE_STATE1:
		// only start motion if the left button is pressed
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{	//start to draw a line
				left_button_down = 1;
				system_state = LINE_STATE2;

				Line newLine;
				newLine.x_1 = x;
				newLine.y_1 = CurrentHeight - y;
				newLine.x_2 = x;
				newLine.y_2 = CurrentHeight - y;
				lines.push_back(newLine);

				glutPostRedisplay();
			}
			else if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
		}
		break;
	case LINE_STATE2:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = LINE_STATE3;
			}
		}
		break;
	case LINE_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - lines[lines.size() - 1].x_1) < 10) && (abs(CurrentHeight - y - lines[lines.size() - 1].y_1) < 10))
				{	//start to edit the line
					lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_2;
					lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_2;
					left_button_down = 1;
					system_state = LINE_STATE2;
				}
				else if ((abs(x - lines[lines.size() - 1].x_2) < 10) && (abs(CurrentHeight - y - lines[lines.size() - 1].y_2) < 10))
				{	//start to edit the line too
					left_button_down = 1;

					system_state = LINE_STATE2;
				}
				else
				{	//start to draw the next line
/*					left_button_down = 1;
					system_state = LINE_STATE2;

					Line newLine;
					newLine.x_1 = x;
					newLine.y_1 = CurrentHeight - y;
					newLine.x_2 = x;
					newLine.y_2 = CurrentHeight - y;
					lines.push_back(newLine);

					glutPostRedisplay();
*/
					system_state = LINE_STATE1;
				}
			}
		}
		break;
	case POLYGON_STATE1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{	//start to draw a polygon
				system_state = POLYGON_STATE2;
				left_button_down = 1;

				vector<Line> newPolygon;

				Line newLine;
				newLine.x_1 = x;
				newLine.y_1 = CurrentHeight - y;
				newLine.x_2 = x;
				newLine.y_2 = CurrentHeight - y;

				newPolygon.push_back(newLine);
				polygons.push_back(newPolygon);

				glutPostRedisplay();

			}
		}
		break;
	case POLYGON_STATE2:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_up = 1;
				left_button_down = 0;
				system_state = POLYGON_STATE3;
			}
		}
		break;
	case POLYGON_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - polygons[polygons.size() - 1][0].x_1) < 5) && (abs(CurrentHeight - y - polygons[polygons.size() - 1][0].y_1) < 5))
				{	//finish drawing the polygon
					system_state = POLYGON_STATE4;
					left_button_up = 0;
					polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = polygons[polygons.size() - 1][0].x_1;
					polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = polygons[polygons.size() - 1][0].y_1;
				}
				else
				{	//start to draw the next edge of the polygon
					system_state = POLYGON_STATE2;
					left_button_down = 1;
					left_button_up = 0;
					Line newLine;

					newLine.x_1 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2;
					newLine.y_1 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2;
					newLine.x_2 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2;
					newLine.y_2 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2;

					polygons[polygons.size() - 1].push_back(newLine);

					glutPostRedisplay();
				}
			}
		}
		break;
	case POLYGON_STATE4:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{	//start to edit the polygon
				system_state = POLYGON_STATE5;
			}
		}
		break;
	case POLYGON_STATE5:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
			else if (state == GLUT_DOWN)
			{	//to edit the polygon
				int i = 0;
				for (; i < polygons[polygons.size() - 1].size(); i++)
				{	//judge whether one vertex of all need to drag
					if ((abs(x - polygons[polygons.size() - 1][i].x_1) < 10) && (abs(CurrentHeight - y - polygons[polygons.size() - 1][i].y_1) < 10))
						break;
				}
				if (i < polygons[polygons.size() - 1].size())
				{
					left_button_down = 1;
					//(i - 1)th and ith edge need to be edited;
					edit_polygon_point = i;
					system_state = POLYGON_STATE5;
				}
				else
				{	//start to draw the next polygon
/*					system_state = POLYGON_STATE2;
					left_button_down = 1;

					vector<Line> newPolygon;

					Line newLine;
					newLine.x_1 = x;
					newLine.y_1 = CurrentHeight - y;
					newLine.x_2 = x;
					newLine.y_2 = CurrentHeight - y;

					newPolygon.push_back(newLine);
					polygons.push_back(newPolygon);

					glutPostRedisplay();
*/
					system_state = POLYGON_STATE1;
				}
			}
		}
		break;
	case ELLIPSE_STATE1://circlebounds follow the order : left, down, right, up
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				left_button_down = 1;

				ellipseBounds[0].x_1 = x;
				ellipseBounds[0].y_1 = CurrentHeight - y;
				ellipseBounds[0].x_2 = x;
				ellipseBounds[0].y_2 = CurrentHeight - y;  //no use

				ellipseBounds[1].x_1 = x;
				ellipseBounds[1].y_1 = CurrentHeight - y;  //no use
				ellipseBounds[1].x_2 = x;  //no use
				ellipseBounds[1].y_2 = CurrentHeight - y;  //no use

				ellipseBounds[2].x_1 = x;  //no use
				ellipseBounds[2].y_1 = CurrentHeight - y;  //no use
				ellipseBounds[2].x_2 = x;  //no use
				ellipseBounds[2].y_2 = CurrentHeight - y;

				ellipseBounds[3].x_1 = x;  //no use
				ellipseBounds[3].y_1 = CurrentHeight - y;
				ellipseBounds[3].x_2 = x;
				ellipseBounds[3].y_2 = CurrentHeight - y;

				Ellipse newEllipse;
				newEllipse.x = (ellipseBounds[0].x_1 + ellipseBounds[2].x_1) / 2;
				newEllipse.y = (ellipseBounds[0].y_1 + ellipseBounds[2].y_1) / 2;
				newEllipse.x_half_length = ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) > 0) ? ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) / 2) : ((ellipseBounds[0].x_1 - ellipseBounds[2].x_1) / 2);
				newEllipse.y_half_length = ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) > 0) ? ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) / 2) : ((ellipseBounds[0].y_1 - ellipseBounds[2].y_1) / 2);
				ellipses.push_back(newEllipse);

				isEllipseEdit = 1;
				system_state = ELLIPSE_STATE2;

				glutPostRedisplay();
			}
			else if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
		}
		break;
	case ELLIPSE_STATE2:  //ready to edit point 2	
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = ELLIPSE_STATE3;
			}
		}
		break;
	case ELLIPSE_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - ellipseBounds[2].x_1) < 10) && (abs(CurrentHeight - y - ellipseBounds[2].y_1) < 10))
				{
					left_button_down = 1;
					system_state = ELLIPSE_STATE2;
				}
				else if ((abs(x - ellipseBounds[0].x_1) < 10) && (abs(CurrentHeight - y - ellipseBounds[0].y_1) < 10))
				{
					left_button_down = 1;
					system_state = ELLIPSE_STATE4;
				}
				else if ((abs(x - ellipseBounds[1].x_1) < 10) && (abs(CurrentHeight - y - ellipseBounds[1].y_1) < 10))
				{
					left_button_down = 1;
					system_state = ELLIPSE_STATE5;
				}
				else if ((abs(x - ellipseBounds[3].x_1) < 10) && (abs(CurrentHeight - y - ellipseBounds[3].y_1) < 10))
				{
					left_button_down = 1;
					system_state = ELLIPSE_STATE6;
				}
				else
				{
					isEllipseEdit = 0;
					system_state = ELLIPSE_STATE1;
				}
			}
		}
		break;
	case ELLIPSE_STATE4:  //ready to edit point 0
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = ELLIPSE_STATE3;
			}
		}
		break;
	case ELLIPSE_STATE5:  //ready to edit point 1
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = ELLIPSE_STATE3;
			}
		}
		break;
	case ELLIPSE_STATE6:  //ready to edit point 3
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = ELLIPSE_STATE3;
			}
		}
		break;
	default: break;
	}
}

void myMotion(int x, int y)
{
	if ((system_state == LINE_STATE1) || (system_state == LINE_STATE2) || (system_state == LINE_STATE3))
	{	//drag one end point of the line when drawing or editing the line
		if (left_button_down == 1)
		{
			lines[lines.size() - 1].x_2 = x;
			lines[lines.size() - 1].y_2 = CurrentHeight - y;
		}
	}
	else if ((system_state == POLYGON_STATE1) || (system_state == POLYGON_STATE2) || (system_state == POLYGON_STATE3))
	{
		if (left_button_down == 1)
		{	//drag one end point of the line when drawing the polygon
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = x;
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = CurrentHeight - y;
		}
	}
	else if (system_state == POLYGON_STATE5)
	{
		if (left_button_down == 1)
		{	//drag one vertex of the polygon when editing the polygon
			if (edit_polygon_point == 0)
			{
				polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = x;
				polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = CurrentHeight - y;
			}
			else
			{
				polygons[polygons.size() - 1][edit_polygon_point - 1].x_2 = x;
				polygons[polygons.size() - 1][edit_polygon_point - 1].y_2 = CurrentHeight - y;
			}
			polygons[polygons.size() - 1][edit_polygon_point].x_1 = x;
			polygons[polygons.size() - 1][edit_polygon_point].y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == ELLIPSE_STATE2)
	{
		if (left_button_down == 1)
		{
			//			ellipseBounds[0].x_1 = x;
			//			ellipseBounds[0].y_1 = CurrentHeight - y;
			//			ellipseBounds[0].x_2 = x;
			ellipseBounds[0].y_2 = CurrentHeight - y;

			//			ellipseBounds[1].x_1 = x;
			ellipseBounds[1].y_1 = CurrentHeight - y;
			ellipseBounds[1].x_2 = x;
			ellipseBounds[1].y_2 = CurrentHeight - y;

			ellipseBounds[2].x_1 = x;
			ellipseBounds[2].y_1 = CurrentHeight - y;
			ellipseBounds[2].x_2 = x;
			//			ellipseBounds[2].y_2 = CurrentHeight - y;

			ellipseBounds[3].x_1 = x;
			//			ellipseBounds[3].y_1 = CurrentHeight - y;
			//			ellipseBounds[3].x_2 = x;
			//			ellipseBounds[3].y_2 = CurrentHeight - y;

			ellipses[ellipses.size() - 1].x = (ellipseBounds[0].x_1 + ellipseBounds[2].x_1) / 2;
			ellipses[ellipses.size() - 1].y = (ellipseBounds[0].y_1 + ellipseBounds[2].y_1) / 2;
			ellipses[ellipses.size() - 1].x_half_length = ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) > 0) ? ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) / 2) : ((ellipseBounds[0].x_1 - ellipseBounds[2].x_1) / 2);
			ellipses[ellipses.size() - 1].y_half_length = ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) > 0) ? ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) / 2) : ((ellipseBounds[0].y_1 - ellipseBounds[2].y_1) / 2);
		}
	}
	else if (system_state == ELLIPSE_STATE4)
	{
		if (left_button_down == 1)
		{
			ellipseBounds[0].x_1 = x;
			ellipseBounds[0].y_1 = CurrentHeight - y;
			ellipseBounds[0].x_2 = x;
			//			ellipseBounds[0].y_2 = CurrentHeight - y;

			ellipseBounds[1].x_1 = x;
			//      	ellipseBounds[1].y_1 = CurrentHeight - y;
			//			ellipseBounds[1].x_2 = x;
			//			ellipseBounds[1].y_2 = CurrentHeight - y;

			//			ellipseBounds[2].x_1 = x;
			//			ellipseBounds[2].y_1 = CurrentHeight - y;
			//			ellipseBounds[2].x_2 = x;
			ellipseBounds[2].y_2 = CurrentHeight - y;

			//			ellipseBounds[3].x_1 = x;
			ellipseBounds[3].y_1 = CurrentHeight - y;
			ellipseBounds[3].x_2 = x;
			ellipseBounds[3].y_2 = CurrentHeight - y;

			ellipses[ellipses.size() - 1].x = (ellipseBounds[0].x_1 + ellipseBounds[2].x_1) / 2;
			ellipses[ellipses.size() - 1].y = (ellipseBounds[0].y_1 + ellipseBounds[2].y_1) / 2;
			ellipses[ellipses.size() - 1].x_half_length = ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) > 0) ? ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) / 2) : ((ellipseBounds[0].x_1 - ellipseBounds[2].x_1) / 2);
			ellipses[ellipses.size() - 1].y_half_length = ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) > 0) ? ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) / 2) : ((ellipseBounds[0].y_1 - ellipseBounds[2].y_1) / 2);
		}
	}
	else if (system_state == ELLIPSE_STATE5)
	{
		if (left_button_down == 1)
		{
			ellipseBounds[0].x_1 = x;
			//			ellipseBounds[0].y_1 = CurrentHeight - y;
			ellipseBounds[0].x_2 = x;
			ellipseBounds[0].y_2 = CurrentHeight - y;

			ellipseBounds[1].x_1 = x;
			ellipseBounds[1].y_1 = CurrentHeight - y;
			//			ellipseBounds[1].x_2 = x;
			ellipseBounds[1].y_2 = CurrentHeight - y;

			//			ellipseBounds[2].x_1 = x;
			ellipseBounds[2].y_1 = CurrentHeight - y;
			//			ellipseBounds[2].x_2 = x;
			//			ellipseBounds[2].y_2 = CurrentHeight - y;

			//			ellipseBounds[3].x_1 = x;
			//			ellipseBounds[3].y_1 = CurrentHeight - y;
			ellipseBounds[3].x_2 = x;
			//			ellipseBounds[3].y_2 = CurrentHeight - y;

			ellipses[ellipses.size() - 1].x = (ellipseBounds[0].x_1 + ellipseBounds[2].x_1) / 2;
			ellipses[ellipses.size() - 1].y = (ellipseBounds[0].y_1 + ellipseBounds[2].y_1) / 2;
			ellipses[ellipses.size() - 1].x_half_length = ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) > 0) ? ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) / 2) : ((ellipseBounds[0].x_1 - ellipseBounds[2].x_1) / 2);
			ellipses[ellipses.size() - 1].y_half_length = ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) > 0) ? ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) / 2) : ((ellipseBounds[0].y_1 - ellipseBounds[2].y_1) / 2);
		}
	}
	else if (system_state == ELLIPSE_STATE6)
	{
		if (left_button_down == 1)
		{
			//			ellipseBounds[0].x_1 = x;
			ellipseBounds[0].y_1 = CurrentHeight - y;
			//			ellipseBounds[0].x_2 = x;
			//			ellipseBounds[0].y_2 = CurrentHeight - y;

			//			ellipseBounds[1].x_1 = x;
			//			ellipseBounds[1].y_1 = CurrentHeight - y;
			ellipseBounds[1].x_2 = x;
			//			ellipseBounds[1].y_2 = CurrentHeight - y;

			ellipseBounds[2].x_1 = x;
			//			ellipseBounds[2].y_1 = CurrentHeight - y;
			ellipseBounds[2].x_2 = x;
			ellipseBounds[2].y_2 = CurrentHeight - y;

			ellipseBounds[3].x_1 = x;
			ellipseBounds[3].y_1 = CurrentHeight - y;
			//			ellipseBounds[3].x_2 = x;
			ellipseBounds[3].y_2 = CurrentHeight - y;

			ellipses[ellipses.size() - 1].x = (ellipseBounds[0].x_1 + ellipseBounds[2].x_1) / 2;
			ellipses[ellipses.size() - 1].y = (ellipseBounds[0].y_1 + ellipseBounds[2].y_1) / 2;
			ellipses[ellipses.size() - 1].x_half_length = ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) > 0) ? ((ellipseBounds[2].x_1 - ellipseBounds[0].x_1) / 2) : ((ellipseBounds[0].x_1 - ellipseBounds[2].x_1) / 2);
			ellipses[ellipses.size() - 1].y_half_length = ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) > 0) ? ((ellipseBounds[2].y_1 - ellipseBounds[0].y_1) / 2) : ((ellipseBounds[0].y_1 - ellipseBounds[2].y_1) / 2);
		}
	}
	glutPostRedisplay();
}

void myPassiveMotion(int x, int y)
{
	if ((system_state == POLYGON_STATE2) || (system_state == POLYGON_STATE3))
	{	//drag one end point of the line when drawing the polygon
		if (left_button_up == 1)
		{
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = x;
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = CurrentHeight - y;
		}
	}
	glutPostRedisplay();
}

void mainMenuProc(int option) {
	switch (option) {
	case LINE:
		system_state = LINE_STATE1;
		break;
	case CIRCLE:
		system_state = CIRCLE_STATE1;
		break;
	case POLYGON:
		system_state = POLYGON_STATE1;
		break;
	case ELLIPSE:
		system_state = ELLIPSE_STATE1;
		break;
	default: break;
	}
}

void createPopupMenus() {

	int mainMenu = glutCreateMenu(mainMenuProc);

	glutAddMenuEntry("Draw lines", LINE);
	glutAddMenuEntry("Draw circles", CIRCLE);
	glutAddMenuEntry("Draw ellipses", ELLIPSE);
	glutAddMenuEntry("Draw polygons", POLYGON);

	/*
	fillMenu = glutCreateMenu(processFillMenu);
	glutAddMenuEntry("Fill", FILL);
	mainMenu = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Polygon Mode", fillMenu);
	*/

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitialWidth, InitialHeight);
	glutCreateWindow("Computer Graphics Lab");

	InitEnvironment();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(myMotion);
	glutPassiveMotionFunc(myPassiveMotion);

	// init Menus
	createPopupMenus();

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}