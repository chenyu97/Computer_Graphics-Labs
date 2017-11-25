#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <list>
#include <GL/glut.h>
using namespace std;

#define InitialWidth 320
#define InitialHeight 320
#define LINE 1
#define LINE_STATE1 101
#define LINE_STATE2 102
#define LINE_STATE3 103
#define LINE_STATE4 104
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
#define FILLEDAREA 5
#define FILLEDAREA_STATE1 501
#define FILLEDAREA_STATE2 502
#define FILLEDAREA_STATE3 503
#define FILLEDAREA_STATE4 504
#define FILLEDAREA_STATE5 505

int system_state = 0;
float CurrentWidth = InitialWidth;
float CurrentHeight = InitialHeight;
int left_button_down = 0;
int left_button_up = 0;
int edit_line_point = -1;
int edit_polygon_point = -1;
int edit_filledArea_point = -1;
int isFilledAreaEdit = 0;
int isFilledAreaEnd = 0;
int isCircleEdit = 0;
int isEllipseEdit = 0;
int resizeLine = 0;
int resizePolygon = 0;
int resizeFilledArea = 0;
int resizeCircle = 0;
int resizeEllipse = 0;

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

struct  Circle
{
	int x;
	int y;
	int r;
};

struct Node{
	float y_max;
	float x;
	float dx;
};

struct floatPoint
{
	float x_1;
	float y_1;
};

vector<list<Node>> ET;
list<Node> AET;

vector<Line> lines;
vector<vector<Line>> polygons;
vector<Ellipse> ellipses;
vector<Circle> circles;
vector<vector<Line>> filledAreas;

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

void drawCircles(Circle c)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	//consider x = 0, y = 0; afterwards we move all the points
	int p0 = 5 / 4 - c.r;
	int x = 0;
	int y = c.r;
	int p = p0;
	while (x <= y)
	{
		glVertex2i(x + c.x, y + c.y);
		glVertex2i(y + c.x, x + c.y);
		glVertex2i((-1) * x + c.x, (-1) * y + c.y);
		glVertex2i((-1) * y + c.x, (-1) * x + c.y);
		glVertex2i((-1) * x + c.x, y + c.y);
		glVertex2i(y + c.x, (-1) * x + c.y);
		glVertex2i(x + c.x, (-1) * y + c.y);
		glVertex2i((-1) * y + c.x, x + c.y);
		if (p < 0)
		{
			p = p + 2 * x + 3;
			x = x + 1;
			y = y;
		}
		else if (p >= 0)
		{
			p = p + 2 * x + 5 - 2 * y;
			x = x + 1;
			y = y - 1;
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

bool compareFun(const Node &first, const Node &second)
{
	return (first.x < second.x);
}

void fillArea(vector<Line> p)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);

	vector<floatPoint> floatPoints;

	//find the y_min and y_max
	int y_min = p[0].y_1;
	int y_max = p[0].y_1;
	for (int i = 0; i < p.size(); i++)
	{
		if (p[i].y_1 < y_min)
			y_min = p[i].y_1;
		if (p[i].y_2 < y_min)
			y_min = p[i].y_2;
		if (p[i].y_1 > y_max)
			y_max = p[i].y_1;
		if (p[i].y_2 > y_max)
			y_max = p[i].y_2;

		floatPoint newFloatPoint;
		newFloatPoint.x_1 = p[i].x_1;
		newFloatPoint.y_1 = p[i].y_1;
		floatPoints.push_back(newFloatPoint);
	}

//	y_min <=> 0, y_max <=> y_max - y_min
	for (int i = y_min; i <= y_max; i++)
	{
		list<Node> nodeList;
		ET.push_back(nodeList);
	}

	for (int i = 0; i <= y_max - y_min; i++)
	{
		for (int j = 0; j < floatPoints.size(); j++)
		{
			if (floatPoints[j].y_1 == y_min + i)
			{
				if (floatPoints[(j - 1 + floatPoints.size()) % (floatPoints.size())].y_1 > floatPoints[j].y_1)
				{
					Node newNode;
					newNode.x = floatPoints[j].x_1;
					newNode.y_max = floatPoints[(j - 1 + floatPoints.size()) % (floatPoints.size())].y_1;
					newNode.dx = (floatPoints[(j - 1 + floatPoints.size()) % (floatPoints.size())].x_1 - floatPoints[j].x_1) / (floatPoints[(j - 1 + floatPoints.size()) % (floatPoints.size())].y_1 - floatPoints[j].y_1);
					ET[i].push_front(newNode);
				}

				if (floatPoints[(j + 1 + floatPoints.size()) % (floatPoints.size())].y_1 > floatPoints[j].y_1)
				{
					Node newNode;
					newNode.x = floatPoints[j].x_1;
					newNode.y_max = floatPoints[(j + 1 + floatPoints.size()) % (floatPoints.size())].y_1;
					newNode.dx = (floatPoints[(j + 1 + floatPoints.size()) % (floatPoints.size())].x_1 - floatPoints[j].x_1) / (floatPoints[(j + 1 + floatPoints.size()) % (floatPoints.size())].y_1 - floatPoints[j].y_1);
					ET[i].push_front(newNode);
				}
			}
		}
	}

	for (int i = 0; i <= y_max - y_min; i++)
	{
		for (list<Node>::iterator it = AET.begin(); it != AET.end(); it++)
		{
			(*it).x += (*it).dx;
		}

		AET.sort(compareFun);

		for (list<Node>::iterator it = AET.begin(); it != AET.end(); )
		{
			if ((*it).y_max == (i + y_min))
			{
				list<Node>::iterator it2 = it;
				it2++;
				AET.erase(it);
				it = it2;
			}
			else
			{
				it++;
			}
		}

		for (list<Node>::iterator it = ET[i].begin(); it != ET[i].end(); it++)
		{
			AET.push_front(*it);
		}
		AET.sort(compareFun);

		for (list<Node>::iterator it = AET.begin(); it != AET.end();)
		{
			list<Node>::iterator it2 = it;
			it2++;
			for (int j = (*it).x; j <= (*(it2)).x; j++)
			{
				glVertex2i(j, i + y_min);
			}
			it++;
			it++;
		}

	}
	for (int i = 0; i < ET.size(); i++)
	{
		ET[i].clear();
	}
	ET.clear();
	AET.clear();

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
		{
			drawLines(polygons[i][j].x_1, polygons[i][j].y_1, polygons[i][j].x_2, polygons[i][j].y_2);
		}
	}

	//draw filledAreas
	for (int i = 0; i < filledAreas.size(); i++)
	{
		for (int j = 0; j < filledAreas[i].size(); j++)
		{
			drawLines(filledAreas[i][j].x_1, filledAreas[i][j].y_1, filledAreas[i][j].x_2, filledAreas[i][j].y_2);
		}

		if ((i != filledAreas.size() - 1) || (isFilledAreaEdit == 1) || (isFilledAreaEnd == 1))
		{
			fillArea(filledAreas[i]);
		}
	}

	//draw circleBounds
	if (isCircleEdit && ((system_state / 100) == 2) && (system_state != CIRCLE_STATE1))
	{
		for (int i = 0; i < 4; i++)
		{
			drawLines(circleBounds[i].x_1, circleBounds[i].y_1, circleBounds[i].x_2, circleBounds[i].y_2);
		}
	}

	//draw circles
	for (int i = 0; i < circles.size(); i++)
	{
		drawCircles(circles[i]);
	}

	//draw ellipseBounds
	if (isEllipseEdit && ((system_state / 100) == 4) && (system_state != ELLIPSE_STATE1))
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
				edit_line_point = -1;
				resizeLine = 0;
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
/*
					lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_2;
					lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_2;
*/					
					edit_line_point = 0;
					left_button_down = 1;
					system_state = LINE_STATE4;
				}
				else if ((abs(x - lines[lines.size() - 1].x_2) < 10) && (abs(CurrentHeight - y - lines[lines.size() - 1].y_2) < 10))
				{	//start to edit the line too
					edit_line_point = 1;
					left_button_down = 1;

					system_state = LINE_STATE4;
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
					edit_line_point = -1;
					resizeLine = 0;
					system_state = LINE_STATE1;
				}
			}
		}
	case LINE_STATE4:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = LINE_STATE3;
			}
		}
		break;
		break;
	case POLYGON_STATE1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{	//start to draw a polygon
				resizePolygon = 0;
				edit_polygon_point = -1;
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
					resizePolygon = 0;
					edit_polygon_point = -1;
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
				resizeEllipse = 0;
				isEllipseEdit = 0;
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
					resizeEllipse = 0;
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
	case CIRCLE_STATE1://circlebounds follow the order : left, down, right, up
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				resizeCircle = 0;
				isCircleEdit = 0;
				left_button_down = 1;

				circleBounds[0].x_1 = x;
				circleBounds[0].y_1 = CurrentHeight - y;
				circleBounds[0].x_2 = x;
				circleBounds[0].y_2 = CurrentHeight - y;  //no use

				circleBounds[1].x_1 = x;
				circleBounds[1].y_1 = CurrentHeight - y;  //no use
				circleBounds[1].x_2 = x;  //no use
				circleBounds[1].y_2 = CurrentHeight - y;  //no use

				circleBounds[2].x_1 = x;  //no use
				circleBounds[2].y_1 = CurrentHeight - y;  //no use
				circleBounds[2].x_2 = x;  //no use
				circleBounds[2].y_2 = CurrentHeight - y;

				circleBounds[3].x_1 = x;  //no use
				circleBounds[3].y_1 = CurrentHeight - y;
				circleBounds[3].x_2 = x;
				circleBounds[3].y_2 = CurrentHeight - y;

				Circle newCircle;
				newCircle.x = (circleBounds[0].x_1 + circleBounds[2].x_1) / 2;
				newCircle.y = (circleBounds[0].y_1 + circleBounds[2].y_1) / 2;
				newCircle.r = ((abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) > (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2)) ? (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) : (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2);
				circles.push_back(newCircle);

				isCircleEdit = 1;
				system_state = CIRCLE_STATE2;

				glutPostRedisplay();
			}
			else if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
		}
		break;
	case CIRCLE_STATE2:  //ready to edit point 2	
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = CIRCLE_STATE3;
			}
		}
		break;
	case CIRCLE_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - circleBounds[2].x_1) < 10) && (abs(CurrentHeight - y - circleBounds[2].y_1) < 10))
				{
					left_button_down = 1;
					system_state = CIRCLE_STATE2;
				}
				else if ((abs(x - circleBounds[0].x_1) < 10) && (abs(CurrentHeight - y - circleBounds[0].y_1) < 10))
				{
					left_button_down = 1;
					system_state = CIRCLE_STATE4;
				}
				else if ((abs(x - circleBounds[1].x_1) < 10) && (abs(CurrentHeight - y - circleBounds[1].y_1) < 10))
				{
					left_button_down = 1;
					system_state = CIRCLE_STATE5;
				}
				else if ((abs(x - circleBounds[3].x_1) < 10) && (abs(CurrentHeight - y - circleBounds[3].y_1) < 10))
				{
					left_button_down = 1;
					system_state = CIRCLE_STATE6;
				}
				else
				{
					resizeCircle = 0;
					isCircleEdit = 0;
					system_state = CIRCLE_STATE1;
				}
			}
		}
		break;
	case CIRCLE_STATE4://ready to edit point 0
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = CIRCLE_STATE3;
			}
		}
		break;
	case CIRCLE_STATE5: //ready to edit point 1
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = CIRCLE_STATE3;
			}
		}
		break;
	case CIRCLE_STATE6: //ready to edit point 3
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = CIRCLE_STATE3;
			}
		}
		break;
	case FILLEDAREA_STATE1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{	//start to draw a filledArea
				resizeFilledArea = 0;
				isFilledAreaEdit = 0;
				system_state = FILLEDAREA_STATE2;
				left_button_down = 1;

				vector<Line> newFilledArea;

				Line newLine;
				newLine.x_1 = x;
				newLine.y_1 = CurrentHeight - y;
				newLine.x_2 = x;
				newLine.y_2 = CurrentHeight - y;

				newFilledArea.push_back(newLine);
				filledAreas.push_back(newFilledArea);

				isFilledAreaEnd = 0;

				glutPostRedisplay();

			}
		}
		break;
	case FILLEDAREA_STATE2:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_up = 1;
				left_button_down = 0;
				system_state = FILLEDAREA_STATE3;
			}
		}
		break;
	case FILLEDAREA_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - filledAreas[filledAreas.size() - 1][0].x_1) < 5) && (abs(CurrentHeight - y - filledAreas[filledAreas.size() - 1][0].y_1) < 5))
				{	//finish drawing the filledArea
					system_state = FILLEDAREA_STATE4;
					left_button_up = 0;
					filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2 = filledAreas[filledAreas.size() - 1][0].x_1;
					filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2 = filledAreas[filledAreas.size() - 1][0].y_1;
				}
				else
				{	//start to draw the next edge of the filledArea
					system_state = FILLEDAREA_STATE2;
					left_button_down = 1;
					left_button_up = 0;
					Line newLine;

					newLine.x_1 = filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2;
					newLine.y_1 = filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2;
					newLine.x_2 = filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2;
					newLine.y_2 = filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2;

					filledAreas[filledAreas.size() - 1].push_back(newLine);

					glutPostRedisplay();
				}
			}
		}
		break;
	case FILLEDAREA_STATE4:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{	//start to edit the filledArea
				system_state = FILLEDAREA_STATE5;
				isFilledAreaEdit = 1;
			}
		}
		break;
	case FILLEDAREA_STATE5:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
			else if (state == GLUT_DOWN)
			{	//to edit the filledArea
				int i = 0;
				for (; i < filledAreas[filledAreas.size() - 1].size(); i++)
				{	//judge whether one vertex of all need to drag
					if ((abs(x - filledAreas[filledAreas.size() - 1][i].x_1) < 10) && (abs(CurrentHeight - y - filledAreas[filledAreas.size() - 1][i].y_1) < 10))
						break;
				}
				if (i < filledAreas[filledAreas.size() - 1].size())
				{
					left_button_down = 1;
					//(i - 1)th and ith edge need to be edited;
					edit_filledArea_point = i;
					system_state = FILLEDAREA_STATE5;
				}
				else
				{	//start to draw the next filledArea
					isFilledAreaEdit = 0;
					isFilledAreaEnd = 1;
					resizeFilledArea = 0;
					system_state = FILLEDAREA_STATE1;
				}
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
	else if (system_state == LINE_STATE4)
	{
		if (left_button_down == 1)
		{
			if (edit_line_point == 0)
			{
				lines[lines.size() - 1].x_1 = x;
				lines[lines.size() - 1].y_1 = CurrentHeight - y;
			}
			else if (edit_line_point == 1)
			{
				lines[lines.size() - 1].x_2 = x;
				lines[lines.size() - 1].y_2 = CurrentHeight - y;
			}
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
	else if ((system_state == FILLEDAREA_STATE1) || (system_state == FILLEDAREA_STATE2) || (system_state == FILLEDAREA_STATE3))
	{
		if (left_button_down == 1)
		{	//drag one end point of the line when drawing the filledArea
			filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2 = x;
			filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2 = CurrentHeight - y;
		}
	}
	else if (system_state == FILLEDAREA_STATE5)
	{
		if (left_button_down == 1)
		{	//drag one vertex of the polygon when editing the filledArea
			if (edit_filledArea_point == 0)
			{
				filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2 = x;
				filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2 = CurrentHeight - y;
			}
			else
			{
				filledAreas[filledAreas.size() - 1][edit_filledArea_point - 1].x_2 = x;
				filledAreas[filledAreas.size() - 1][edit_filledArea_point - 1].y_2 = CurrentHeight - y;
			}
			filledAreas[filledAreas.size() - 1][edit_filledArea_point].x_1 = x;
			filledAreas[filledAreas.size() - 1][edit_filledArea_point].y_1 = CurrentHeight - y;
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
	else if (system_state == CIRCLE_STATE2)
	{
		if (left_button_down == 1)
		{
			int max = (abs(x - circleBounds[0].x_1) > abs(CurrentHeight - y - circleBounds[0].y_1)) ? abs(x - circleBounds[0].x_1) : abs(CurrentHeight - y - circleBounds[0].y_1);

			//circleBounds[0].x_1 = x;
			//circleBounds[0].y_1 = CurrentHeight - y;
			//circleBounds[0].x_2 = x;
			circleBounds[0].y_2 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max); 

			//circleBounds[1].x_1 = x;
			circleBounds[1].y_1 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max); 
			circleBounds[1].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			circleBounds[1].y_2 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);

			circleBounds[2].x_1 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			circleBounds[2].y_1 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);
			circleBounds[2].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			//circleBounds[2].y_2 = CurrentHeight - y;

			circleBounds[3].x_1 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			//circleBounds[3].y_1 = CurrentHeight - y;
			//circleBounds[3].x_2 = x;
			//circleBounds[3].y_2 = CurrentHeight - y;

			circles[circles.size() - 1].x = (circleBounds[0].x_1 + circleBounds[2].x_1) / 2;
			circles[circles.size() - 1].y = (circleBounds[0].y_1 + circleBounds[2].y_1) / 2;
			circles[circles.size() - 1].r = (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) > (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2) ? (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) : (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2);
		}
	}
	else if (system_state == CIRCLE_STATE4)
	{
		if (left_button_down == 1)
		{
			int max = (abs(x - circleBounds[2].x_1) > abs(CurrentHeight - y - circleBounds[2].y_1)) ? abs(x - circleBounds[2].x_1) : abs(CurrentHeight - y - circleBounds[2].y_1);

			circleBounds[0].x_1 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			circleBounds[0].y_1 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);
			circleBounds[0].x_2 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			//circleBounds[0].y_2 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);

			circleBounds[1].x_1 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			//circleBounds[1].y_1 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);
			//circleBounds[1].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			//circleBounds[1].y_2 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);

			//circleBounds[2].x_1 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			//circleBounds[2].y_1 = (CurrentHeight - y - circleBounds[0].y_1 > 0) ? (circleBounds[0].y_1 + max) : (circleBounds[0].y_1 - max);
			//circleBounds[2].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			circleBounds[2].y_2 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);

			//circleBounds[3].x_1 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			circleBounds[3].y_1 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);
			circleBounds[3].x_2 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			circleBounds[3].y_2 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);

			circles[circles.size() - 1].x = (circleBounds[0].x_1 + circleBounds[2].x_1) / 2;
			circles[circles.size() - 1].y = (circleBounds[0].y_1 + circleBounds[2].y_1) / 2;
			circles[circles.size() - 1].r = (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) > (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2) ? (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) : (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2);
		}
	}
	else if (system_state == CIRCLE_STATE5)
	{
		if (left_button_down == 1)
		{
			int max = (abs(x - circleBounds[3].x_1) > abs(CurrentHeight - y - circleBounds[3].y_1)) ? abs(x - circleBounds[3].x_1) : abs(CurrentHeight - y - circleBounds[3].y_1);

			circleBounds[0].x_1 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			//circleBounds[0].y_1 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);
			circleBounds[0].x_2 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			circleBounds[0].y_2 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);

			circleBounds[1].x_1 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			circleBounds[1].y_1 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);
			//circleBounds[1].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			circleBounds[1].y_2 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);

			//circleBounds[2].x_1 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			circleBounds[2].y_1 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);
			//circleBounds[2].x_2 = (x - circleBounds[0].x_1 > 0) ? (circleBounds[0].x_1 + max) : (circleBounds[0].x_1 - max);
			//circleBounds[2].y_2 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);

			//circleBounds[3].x_1 = (x - circleBounds[2].x_1 > 0) ? (circleBounds[2].x_1 + max) : (circleBounds[2].x_1 - max);
			//circleBounds[3].y_1 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);
			circleBounds[3].x_2 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			//circleBounds[3].y_2 = (CurrentHeight - y - circleBounds[2].y_1 > 0) ? (circleBounds[2].y_1 + max) : (circleBounds[2].y_1 - max);

			circles[circles.size() - 1].x = (circleBounds[0].x_1 + circleBounds[2].x_1) / 2;
			circles[circles.size() - 1].y = (circleBounds[0].y_1 + circleBounds[2].y_1) / 2;
			circles[circles.size() - 1].r = (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) > (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2) ? (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) : (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2);
		}
	}
	else if (system_state == CIRCLE_STATE6)
	{
		if (left_button_down == 1)
		{
			int max = (abs(x - circleBounds[1].x_1) > abs(CurrentHeight - y - circleBounds[1].y_1)) ? abs(x - circleBounds[1].x_1) : abs(CurrentHeight - y - circleBounds[1].y_1);

			//circleBounds[0].x_1 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			circleBounds[0].y_1 = (CurrentHeight - y - circleBounds[1].y_1 > 0) ? (circleBounds[1].y_1 + max) : (circleBounds[1].y_1 - max);
			//circleBounds[0].x_2 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			//circleBounds[0].y_2 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);

			//circleBounds[1].x_1 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			//circleBounds[1].y_1 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);
			circleBounds[1].x_2 = (x - circleBounds[1].x_1 > 0) ? (circleBounds[1].x_1 + max) : (circleBounds[1].x_1 - max);
			//circleBounds[1].y_2 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);

			circleBounds[2].x_1 = (x - circleBounds[1].x_1 > 0) ? (circleBounds[1].x_1 + max) : (circleBounds[1].x_1 - max);
			//circleBounds[2].y_1 = (CurrentHeight - y - circleBounds[3].y_1 > 0) ? (circleBounds[3].y_1 + max) : (circleBounds[3].y_1 - max);
			circleBounds[2].x_2 = (x - circleBounds[1].x_1 > 0) ? (circleBounds[1].x_1 + max) : (circleBounds[1].x_1 - max);
			circleBounds[2].y_2 = (CurrentHeight - y - circleBounds[1].y_1 > 0) ? (circleBounds[1].y_1 + max) : (circleBounds[1].y_1 - max);

			circleBounds[3].x_1 = (x - circleBounds[1].x_1 > 0) ? (circleBounds[1].x_1 + max) : (circleBounds[1].x_1 - max);
			circleBounds[3].y_1 = (CurrentHeight - y - circleBounds[1].y_1 > 0) ? (circleBounds[1].y_1 + max) : (circleBounds[1].y_1 - max);
			//circleBounds[3].x_2 = (x - circleBounds[3].x_1 > 0) ? (circleBounds[3].x_1 + max) : (circleBounds[3].x_1 - max);
			circleBounds[3].y_2 = (CurrentHeight - y - circleBounds[1].y_1 > 0) ? (circleBounds[1].y_1 + max) : (circleBounds[1].y_1 - max);

			circles[circles.size() - 1].x = (circleBounds[0].x_1 + circleBounds[2].x_1) / 2;
			circles[circles.size() - 1].y = (circleBounds[0].y_1 + circleBounds[2].y_1) / 2;
			circles[circles.size() - 1].r = (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) > (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2) ? (abs(circleBounds[2].x_1 - circleBounds[0].x_1) / 2) : (abs(circleBounds[2].y_1 - circleBounds[0].y_1) / 2);
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
	else if ((system_state == FILLEDAREA_STATE2) || (system_state == FILLEDAREA_STATE3))
	{	//drag one end point of the line when drawing the filledArea
		if (left_button_up == 1)
		{
			filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].x_2 = x;
			filledAreas[filledAreas.size() - 1][filledAreas[filledAreas.size() - 1].size() - 1].y_2 = CurrentHeight - y;
		}
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	switch (system_state){
	case LINE_STATE3:
		if (key == 'a')
		{
			lines[lines.size() - 1].x_1 -= 1;
			lines[lines.size() - 1].x_2 -= 1;
		}
		else if (key == 'd')
		{
			lines[lines.size() - 1].x_1 += 1;
			lines[lines.size() - 1].x_2 += 1;
		}
		else if (key == 'w')
		{
			lines[lines.size() - 1].y_1 += 1;
			lines[lines.size() - 1].y_2 += 1;
		}
		else if (key == 's')
		{
			lines[lines.size() - 1].y_1 -= 1;
			lines[lines.size() - 1].y_2 -= 1;
		}
		else if (key == 'q')
		{
			int x1 = lines[lines.size() - 1].x_1;
			int y1 = lines[lines.size() - 1].y_1;
			int x2 = lines[lines.size() - 1].x_2;
			int y2 = lines[lines.size() - 1].y_2;
			lines[lines.size() - 1].x_1 = (x1 - x1) * 0 - (y1 - y1)* 1 + x1;
			lines[lines.size() - 1].y_1 = (x1 - x1) * 1 + (y1 - y1) * 0 + y1;
			lines[lines.size() - 1].x_2 = (x2 - x1) * 0 - (y2 - y1) * 1 + x1;
			lines[lines.size() - 1].y_2 = (x2 - x1) * 1 + (y2 - y1) * 0 + y1;
		}
		else if (key == 'e')
		{
			int x1 = lines[lines.size() - 1].x_1;
			int y1 = lines[lines.size() - 1].y_1;
			int x2 = lines[lines.size() - 1].x_2;
			int y2 = lines[lines.size() - 1].y_2;
			lines[lines.size() - 1].x_1 = (x1 - x1) * 0 - (y1 - y1) * (-1) + x1;
			lines[lines.size() - 1].y_1 = (x1 - x1) * (-1) + (y1 - y1) * 0 + y1;
			lines[lines.size() - 1].x_2 = (x2 - x1) * 0 - (y2 - y1) * (-1) + x1;
			lines[lines.size() - 1].y_2 = (x2 - x1) * (-1) + (y2 - y1) * 0 + y1;
		}
		else if (key == 'z')
		{
			if (resizeLine > 0)
			{
				lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_1 / 2;
				lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_1 / 2;
				lines[lines.size() - 1].x_2 = lines[lines.size() - 1].x_2 / 2;
				lines[lines.size() - 1].y_2 = lines[lines.size() - 1].y_2 / 2;
				resizeLine--;
			}
		}
		else if (key == 'x')
		{
			lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_1 * 2;
			lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_1 * 2;
			lines[lines.size() - 1].x_2 = lines[lines.size() - 1].x_2 * 2;
			lines[lines.size() - 1].y_2 = lines[lines.size() - 1].y_2 * 2;
			resizeLine++;
		}
		break;
	case POLYGON_STATE5:
		if (key == 'a')
		{
			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].x_1 -= 1;
				polygons[polygons.size() - 1][i].x_2 -= 1;
			}
		}
		else if (key == 'd')
		{
			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].x_1 += 1;
				polygons[polygons.size() - 1][i].x_2 += 1;
			}
		}
		else if (key == 'w')
		{
			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].y_1 += 1;
				polygons[polygons.size() - 1][i].y_2 += 1;
			}
		}
		else if (key == 's')
		{
			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].y_1 -= 1;
				polygons[polygons.size() - 1][i].y_2 -= 1;
			}
		}
		else if (key == 'q')
		{
			vector<vector<Line>> temp = polygons;
			int x0 = polygons[polygons.size() - 1][0].x_1;
			int y0 = polygons[polygons.size() - 1][0].y_1;

			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].x_1 = (temp[temp.size() - 1][i].x_1 - x0) * 0 - (temp[temp.size() - 1][i].y_1 - y0) * 1 + x0;
				polygons[polygons.size() - 1][i].y_1 = (temp[temp.size() - 1][i].x_1 - x0) * 1 + (temp[temp.size() - 1][i].y_1 - y0) * 0 + y0;
				polygons[polygons.size() - 1][i].x_2 = (temp[temp.size() - 1][i].x_2 - x0) * 0 - (temp[temp.size() - 1][i].y_2 - y0) * 1 + x0;
				polygons[polygons.size() - 1][i].y_2 = (temp[temp.size() - 1][i].x_2 - x0) * 1 + (temp[temp.size() - 1][i].y_2 - y0) * 0 + y0;
			}

			for (int i = 0; i < temp.size(); i++)
			{
				temp[i].clear();
			}
			temp.clear();
		}
		else if (key == 'e')
		{
			vector<vector<Line>> temp = polygons;
			int x0 = polygons[polygons.size() - 1][0].x_1;
			int y0 = polygons[polygons.size() - 1][0].y_1;

			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].x_1 = (temp[temp.size() - 1][i].x_1 - x0) * 0 - (temp[temp.size() - 1][i].y_1 - y0) * (-1) + x0;
				polygons[polygons.size() - 1][i].y_1 = (temp[temp.size() - 1][i].x_1 - x0) * (-1) + (temp[temp.size() - 1][i].y_1 - y0) * 0 + y0;
				polygons[polygons.size() - 1][i].x_2 = (temp[temp.size() - 1][i].x_2 - x0) * 0 - (temp[temp.size() - 1][i].y_2 - y0) * (-1) + x0;
				polygons[polygons.size() - 1][i].y_2 = (temp[temp.size() - 1][i].x_2 - x0) * (-1) + (temp[temp.size() - 1][i].y_2 - y0) * 0 + y0;
			}

			for (int i = 0; i < temp.size(); i++)
			{
				temp[i].clear();
			}
			temp.clear();
		}
		else if (key == 'z')
		{
			if (resizePolygon > 0)
			{
				for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
				{
					polygons[polygons.size() - 1][i].x_1 = polygons[polygons.size() - 1][i].x_1 / 2;
					polygons[polygons.size() - 1][i].y_1 = polygons[polygons.size() - 1][i].y_1 / 2;
					polygons[polygons.size() - 1][i].x_2 = polygons[polygons.size() - 1][i].x_2 / 2;
					polygons[polygons.size() - 1][i].y_2 = polygons[polygons.size() - 1][i].y_2 / 2;
				}
				resizePolygon--;
			}
		}
		else if (key == 'x')
		{
			for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
			{
				polygons[polygons.size() - 1][i].x_1 = polygons[polygons.size() - 1][i].x_1 * 2;
				polygons[polygons.size() - 1][i].y_1 = polygons[polygons.size() - 1][i].y_1 * 2;
				polygons[polygons.size() - 1][i].x_2 = polygons[polygons.size() - 1][i].x_2 * 2;
				polygons[polygons.size() - 1][i].y_2 = polygons[polygons.size() - 1][i].y_2 * 2;
			}
			resizePolygon++;
		}
		break;
	case FILLEDAREA_STATE5:
		if (key == 'a')
		{
			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].x_1 -= 1;
				filledAreas[filledAreas.size() - 1][i].x_2 -= 1;
			}
		}
		else if (key == 'd')
		{
			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].x_1 += 1;
				filledAreas[filledAreas.size() - 1][i].x_2 += 1;
			}
		}
		else if (key == 'w')
		{
			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].y_1 += 1;
				filledAreas[filledAreas.size() - 1][i].y_2 += 1;
			}
		}
		else if (key == 's')
		{
			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].y_1 -= 1;
				filledAreas[filledAreas.size() - 1][i].y_2 -= 1;
			}
		}
		else if (key == 'q')
		{
			vector<vector<Line>> temp = filledAreas;
			int x0 = filledAreas[filledAreas.size() - 1][0].x_1;
			int y0 = filledAreas[filledAreas.size() - 1][0].y_1;

			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].x_1 = (temp[temp.size() - 1][i].x_1 - x0) * 0 - (temp[temp.size() - 1][i].y_1 - y0) * 1 + x0;
				filledAreas[filledAreas.size() - 1][i].y_1 = (temp[temp.size() - 1][i].x_1 - x0) * 1 + (temp[temp.size() - 1][i].y_1 - y0) * 0 + y0;
				filledAreas[filledAreas.size() - 1][i].x_2 = (temp[temp.size() - 1][i].x_2 - x0) * 0 - (temp[temp.size() - 1][i].y_2 - y0) * 1 + x0;
				filledAreas[filledAreas.size() - 1][i].y_2 = (temp[temp.size() - 1][i].x_2 - x0) * 1 + (temp[temp.size() - 1][i].y_2 - y0) * 0 + y0;
			}

			for (int i = 0; i < temp.size(); i++)
			{
				temp[i].clear();
			}
			temp.clear();
		}
		else if (key == 'e')
		{
			vector<vector<Line>> temp = filledAreas;
			int x0 = filledAreas[filledAreas.size() - 1][0].x_1;
			int y0 = filledAreas[filledAreas.size() - 1][0].y_1;

			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].x_1 = (temp[temp.size() - 1][i].x_1 - x0) * 0 - (temp[temp.size() - 1][i].y_1 - y0) * (-1) + x0;
				filledAreas[filledAreas.size() - 1][i].y_1 = (temp[temp.size() - 1][i].x_1 - x0) * (-1) + (temp[temp.size() - 1][i].y_1 - y0) * 0 + y0;
				filledAreas[filledAreas.size() - 1][i].x_2 = (temp[temp.size() - 1][i].x_2 - x0) * 0 - (temp[temp.size() - 1][i].y_2 - y0) * (-1) + x0;
				filledAreas[filledAreas.size() - 1][i].y_2 = (temp[temp.size() - 1][i].x_2 - x0) * (-1) + (temp[temp.size() - 1][i].y_2 - y0) * 0 + y0;
			}

			for (int i = 0; i < temp.size(); i++)
			{
				temp[i].clear();
			}
			temp.clear();
		}
		else if (key == 'z')
		{
			if (resizeFilledArea > 0)
			{
				for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
				{
					filledAreas[filledAreas.size() - 1][i].x_1 = filledAreas[filledAreas.size() - 1][i].x_1 / 2;
					filledAreas[filledAreas.size() - 1][i].y_1 = filledAreas[filledAreas.size() - 1][i].y_1 / 2;
					filledAreas[filledAreas.size() - 1][i].x_2 = filledAreas[filledAreas.size() - 1][i].x_2 / 2;
					filledAreas[filledAreas.size() - 1][i].y_2 = filledAreas[filledAreas.size() - 1][i].y_2 / 2;
				}
				resizeFilledArea--;
			}
		}
		else if (key == 'x')
		{
			for (int i = 0; i < filledAreas[filledAreas.size() - 1].size(); i++)
			{
				filledAreas[filledAreas.size() - 1][i].x_1 = filledAreas[filledAreas.size() - 1][i].x_1 * 2;
				filledAreas[filledAreas.size() - 1][i].y_1 = filledAreas[filledAreas.size() - 1][i].y_1 * 2;
				filledAreas[filledAreas.size() - 1][i].x_2 = filledAreas[filledAreas.size() - 1][i].x_2 * 2;
				filledAreas[filledAreas.size() - 1][i].y_2 = filledAreas[filledAreas.size() - 1][i].y_2 * 2;
			}
			resizeFilledArea++;
		}
		break;
	case ELLIPSE_STATE3:
		if (key == 'a')
		{
			ellipses[ellipses.size() - 1].x -= 1;
			for (int i = 0; i < 4; i++)
			{
				ellipseBounds[i].x_1 -= 1;
				ellipseBounds[i].x_2 -= 1;
			}
		}
		else if (key == 'd')
		{
			ellipses[ellipses.size() - 1].x += 1;
			for (int i = 0; i < 4; i++)
			{
				ellipseBounds[i].x_1 += 1;
				ellipseBounds[i].x_2 += 1;
			}
		}
		else if (key == 'w')
		{
			ellipses[ellipses.size() - 1].y += 1;
			for (int i = 0; i < 4; i++)
			{
				ellipseBounds[i].y_1 += 1;
				ellipseBounds[i].y_2 += 1;
			}
		}
		else if (key == 's')
		{
			ellipses[ellipses.size() - 1].y -= 1;
			for (int i = 0; i < 4; i++)
			{
				ellipseBounds[i].y_1 -= 1;
				ellipseBounds[i].y_2 -= 1;
			}
		}
		else if (key == 'q')
		{
			int temp = ellipses[ellipses.size() - 1].x_half_length;
			ellipses[ellipses.size() - 1].x_half_length = ellipses[ellipses.size() - 1].y_half_length;
			ellipses[ellipses.size() - 1].y_half_length = temp;
			//rotate the bounds		
			int x01 = ellipseBounds[0].x_1;
			int y01 = ellipseBounds[0].y_1;
			int x02 = ellipseBounds[0].x_2;
			int y02 = ellipseBounds[0].y_2;
			int x11 = ellipseBounds[1].x_1;
			int y11 = ellipseBounds[1].y_1;
			int x12 = ellipseBounds[1].x_2;
			int y12 = ellipseBounds[1].y_2;
			int x21 = ellipseBounds[2].x_1;
			int y21 = ellipseBounds[2].y_1;
			int x22 = ellipseBounds[2].x_2;
			int y22 = ellipseBounds[2].y_2;
			int x31 = ellipseBounds[3].x_1;
			int y31 = ellipseBounds[3].y_1;
			int x32 = ellipseBounds[3].x_2;
			int y32 = ellipseBounds[3].y_2;

			int x0 = ellipses[ellipses.size() - 1].x;
			int y0 = ellipses[ellipses.size() - 1].y;

			ellipseBounds[0].x_1 = (x01 - x0) * 0 - (y01 - y0) * 1 + x0;
			ellipseBounds[0].y_1 = (x01 - x0) * 1 + (y01 - y0) * 0 + y0;
			ellipseBounds[0].x_2 = (x02 - x0) * 0 - (y02 - y0) * 1 + x0;
			ellipseBounds[0].y_2 = (x02 - x0) * 1 + (y02 - y0) * 0 + y0;
			ellipseBounds[1].x_1 = (x11 - x0) * 0 - (y11 - y0) * 1 + x0;
			ellipseBounds[1].y_1 = (x11 - x0) * 1 + (y11 - y0) * 0 + y0;
			ellipseBounds[1].x_2 = (x12 - x0) * 0 - (y12 - y0) * 1 + x0;
			ellipseBounds[1].y_2 = (x12 - x0) * 1 + (y12 - y0) * 0 + y0;
			ellipseBounds[2].x_1 = (x21 - x0) * 0 - (y21 - y0) * 1 + x0;
			ellipseBounds[2].y_1 = (x21 - x0) * 1 + (y21 - y0) * 0 + y0;
			ellipseBounds[2].x_2 = (x22 - x0) * 0 - (y22 - y0) * 1 + x0;
			ellipseBounds[2].y_2 = (x22 - x0) * 1 + (y22 - y0) * 0 + y0;
			ellipseBounds[3].x_1 = (x31 - x0) * 0 - (y31 - y0) * 1 + x0;
			ellipseBounds[3].y_1 = (x31 - x0) * 1 + (y31 - y0) * 0 + y0;
			ellipseBounds[3].x_2 = (x32 - x0) * 0 - (y32 - y0) * 1 + x0;
			ellipseBounds[3].y_2 = (x32 - x0) * 1 + (y32 - y0) * 0 + y0;
		}
		else if (key == 'e')
		{
			int temp = ellipses[ellipses.size() - 1].x_half_length;
			ellipses[ellipses.size() - 1].x_half_length = ellipses[ellipses.size() - 1].y_half_length;
			ellipses[ellipses.size() - 1].y_half_length = temp;
			//rotate the bounds
			int x01 = ellipseBounds[0].x_1;
			int y01 = ellipseBounds[0].y_1;
			int x02 = ellipseBounds[0].x_2;
			int y02 = ellipseBounds[0].y_2;
			int x11 = ellipseBounds[1].x_1;
			int y11 = ellipseBounds[1].y_1;
			int x12 = ellipseBounds[1].x_2;
			int y12 = ellipseBounds[1].y_2;
			int x21 = ellipseBounds[2].x_1;
			int y21 = ellipseBounds[2].y_1;
			int x22 = ellipseBounds[2].x_2;
			int y22 = ellipseBounds[2].y_2;
			int x31 = ellipseBounds[3].x_1;
			int y31 = ellipseBounds[3].y_1;
			int x32 = ellipseBounds[3].x_2;
			int y32 = ellipseBounds[3].y_2;

			int x0 = ellipses[ellipses.size() - 1].x;
			int y0 = ellipses[ellipses.size() - 1].y;

			ellipseBounds[0].x_1 = (x01 - x0) * 0 - (y01 - y0) * (-1) + x0;
			ellipseBounds[0].y_1 = (x01 - x0) * (-1) + (y01 - y0) * 0 + y0;
			ellipseBounds[0].x_2 = (x02 - x0) * 0 - (y02 - y0) * (-1) + x0;
			ellipseBounds[0].y_2 = (x02 - x0) * (-1) + (y02 - y0) * 0 + y0;
			ellipseBounds[1].x_1 = (x11 - x0) * 0 - (y11 - y0) * (-1) + x0;
			ellipseBounds[1].y_1 = (x11 - x0) * (-1) + (y11 - y0) * 0 + y0;
			ellipseBounds[1].x_2 = (x12 - x0) * 0 - (y12 - y0) * (-1) + x0;
			ellipseBounds[1].y_2 = (x12 - x0) * (-1) + (y12 - y0) * 0 + y0;
			ellipseBounds[2].x_1 = (x21 - x0) * 0 - (y21 - y0) * (-1) + x0;
			ellipseBounds[2].y_1 = (x21 - x0) * (-1) + (y21 - y0) * 0 + y0;
			ellipseBounds[2].x_2 = (x22 - x0) * 0 - (y22 - y0) * (-1) + x0;
			ellipseBounds[2].y_2 = (x22 - x0) * (-1) + (y22 - y0) * 0 + y0;
			ellipseBounds[3].x_1 = (x31 - x0) * 0 - (y31 - y0) * (-1) + x0;
			ellipseBounds[3].y_1 = (x31 - x0) * (-1) + (y31 - y0) * 0 + y0;
			ellipseBounds[3].x_2 = (x32 - x0) * 0 - (y32 - y0) * (-1) + x0;
			ellipseBounds[3].y_2 = (x32 - x0) * (-1) + (y32 - y0) * 0 + y0;
		}
		else if (key == 'z')
		{
			if (resizeEllipse > 0)
			{
				ellipses[ellipses.size() - 1].x_half_length = ellipses[ellipses.size() - 1].x_half_length / 2;
				ellipses[ellipses.size() - 1].y_half_length = ellipses[ellipses.size() - 1].y_half_length / 2;
				for (int i = 0; i < 4; i++)
				{
					ellipseBounds[i].x_1 = (ellipseBounds[i].x_1 - ellipses[ellipses.size() - 1].x) / 2 + ellipses[ellipses.size() - 1].x;
					ellipseBounds[i].y_1 = (ellipseBounds[i].y_1 - ellipses[ellipses.size() - 1].y) / 2 + ellipses[ellipses.size() - 1].y;
					ellipseBounds[i].x_2 = (ellipseBounds[i].x_2 - ellipses[ellipses.size() - 1].x) / 2 + ellipses[ellipses.size() - 1].x;
					ellipseBounds[i].y_2 = (ellipseBounds[i].y_2 - ellipses[ellipses.size() - 1].y) / 2 + ellipses[ellipses.size() - 1].y;
				}
				resizeEllipse--;
			}
		}
		else if (key == 'x')
		{
			if (resizeEllipse < 1) //Although we have this limit, something strange will still happen when the ellipse is large to some degree.
			{
				ellipses[ellipses.size() - 1].x_half_length = ellipses[ellipses.size() - 1].x_half_length * 2;
				ellipses[ellipses.size() - 1].y_half_length = ellipses[ellipses.size() - 1].y_half_length * 2;
				for (int i = 0; i < 4; i++)
				{
					ellipseBounds[i].x_1 = (ellipseBounds[i].x_1 - ellipses[ellipses.size() - 1].x) * 2 + ellipses[ellipses.size() - 1].x;
					ellipseBounds[i].y_1 = (ellipseBounds[i].y_1 - ellipses[ellipses.size() - 1].y) * 2 + ellipses[ellipses.size() - 1].y;
					ellipseBounds[i].x_2 = (ellipseBounds[i].x_2 - ellipses[ellipses.size() - 1].x) * 2 + ellipses[ellipses.size() - 1].x;
					ellipseBounds[i].y_2 = (ellipseBounds[i].y_2 - ellipses[ellipses.size() - 1].y) * 2 + ellipses[ellipses.size() - 1].y;
				}
				resizeEllipse++;
			}
		}
		break;
	case CIRCLE_STATE3:
		if (key == 'a')
		{
			circles[circles.size() - 1].x -= 1;
			for (int i = 0; i < 4; i++)
			{
				circleBounds[i].x_1 -= 1;
				circleBounds[i].x_2 -= 1;
			}
		}
		else if (key == 'd')
		{
			circles[circles.size() - 1].x += 1;
			for (int i = 0; i < 4; i++)
			{
				circleBounds[i].x_1 += 1;
				circleBounds[i].x_2 += 1;
			}
		}
		else if (key == 'w')
		{
			circles[circles.size() - 1].y += 1;
			for (int i = 0; i < 4; i++)
			{
				circleBounds[i].y_1 += 1;
				circleBounds[i].y_2 += 1;
			}
		}
		else if (key == 's')
		{
			circles[circles.size() - 1].y -= 1;
			for (int i = 0; i < 4; i++)
			{
				circleBounds[i].y_1 -= 1;
				circleBounds[i].y_2 -= 1;
			}
		}
		else if (key == 'q')
		{
			;//nothing needs doing to rotate circles
		}
		else if (key == 'e')
		{
			;//nothing needs doing to rotate circles
		}
		else if (key == 'z')
		{
			if (resizeCircle > 0)
			{
				circles[circles.size() - 1].r = circles[circles.size() - 1].r / 2;
				for (int i = 0; i < 4; i++)
				{
					circleBounds[i].x_1 = (circleBounds[i].x_1 - circles[circles.size() - 1].x) / 2 + circles[circles.size() - 1].x;
					circleBounds[i].y_1 = (circleBounds[i].y_1 - circles[circles.size() - 1].y) / 2 + circles[circles.size() - 1].y;
					circleBounds[i].x_2 = (circleBounds[i].x_2 - circles[circles.size() - 1].x) / 2 + circles[circles.size() - 1].x;
					circleBounds[i].y_2 = (circleBounds[i].y_2 - circles[circles.size() - 1].y) / 2 + circles[circles.size() - 1].y;
				}
				resizeCircle--;
			}
		}
		else if (key == 'x')
		{
			if (resizeCircle < 1)
			{
				circles[circles.size() - 1].r = circles[circles.size() - 1].r * 2;
				for (int i = 0; i < 4; i++)
				{
					circleBounds[i].x_1 = (circleBounds[i].x_1 - circles[circles.size() - 1].x) * 2 + circles[circles.size() - 1].x;
					circleBounds[i].y_1 = (circleBounds[i].y_1 - circles[circles.size() - 1].y) * 2 + circles[circles.size() - 1].y;
					circleBounds[i].x_2 = (circleBounds[i].x_2 - circles[circles.size() - 1].x) * 2 + circles[circles.size() - 1].x;
					circleBounds[i].y_2 = (circleBounds[i].y_2 - circles[circles.size() - 1].y) * 2 + circles[circles.size() - 1].y;
				}
				resizeCircle++;
			}
		}
		break;
	default: break;
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
	case FILLEDAREA:
		system_state = FILLEDAREA_STATE1;
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
	glutAddMenuEntry("Draw filledAreas", FILLEDAREA);
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
	glutKeyboardFunc(processNormalKeys);

	// init Menus
	createPopupMenus();

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}