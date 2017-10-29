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
#define POLYGON 3
#define POLYGON_STATE1 301
#define POLYGON_STATE2 302
#define POLYGON_STATE3 303
#define POLYGON_STATE4 304
#define POLYGON_STATE5 305

int system_state = 0;
float CurrentWidth = InitialWidth;
float CurrentHeight = InitialHeight;
int left_button_down = 0;
int left_button_up = 0;
int edit_polygon_point = -1;

struct Line
{
	int x_1;
	int y_1;
	int x_2;
	int y_2;
};
vector<Line> lines;

vector<vector<Line>> polygons;

void drawLines(int x_1, int y_1, int x_2, int y_2)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	
	int dx, dy;
	dx = x_2 - x_1;
	dy = y_2 - y_1;

	if (x_1 == x_2)
	{
		int min = (y_1 <= y_2) ? y_1 : y_2;
		int max = y_1 + y_2 - min;
		for (int i = min; i <= max; i++)
		{
			glVertex2i(x_1, i);
		}
	}
	else if (y_1 == y_2)
	{
		int min = (x_1 <= x_2) ? x_1 : x_2;
		int max = x_1 + x_2 - min;
		for (int i = min; i <= max; i++)
		{
			glVertex2i(i, y_1);
		}
	}
	else if (abs(dx) >= abs(dy))
	{
		if ((dx > 0) == (dy > 0))
		{
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_1;
				x_min = x_2;
				y_min = y_2;
//				cout << '(' << x_min << ',' << y_min << ") ,(" << x_max << ',' << y_max << ") = =" << endl;
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
		{
			int x_max, y_max, x_min, y_min;
			if (x_1 > x_2)
			{
				x_max = x_1;
				y_max = y_2;
				x_min = x_2;
				y_min = y_1;

				cout << '(' << x_min << ',' << y_min << ") ,(" << x_max << ',' << y_max << ") = =" << endl;

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
	else if(abs(dx) < abs(dy))
	{
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
//				cout << '(' << x_min << ',' << y_min << ") ,(" << x_max << ',' << y_max << ") = =" << endl;
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

void InitEnvironment()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glViewport(0, 0, InitialWidth, InitialHeight);
//	gluOrtho2D(0, InitialWidth, 0, InitialHeight);

	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH) * 2, glutGet(GLUT_SCREEN_HEIGHT) * 2);
	gluOrtho2D(0, glutGet(GLUT_SCREEN_WIDTH) * 2, 0, glutGet(GLUT_SCREEN_HEIGHT) * 2);

//	gluPerspective(45, 1.0 * WIDTH / HEIGHT, 1, 100);
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

	for (int i = 0; i < lines.size(); i++)
	{
		cout << '(' << lines[i].x_1 << ',' << lines[i].y_1 << ") ,(" << lines[i].x_2  << ',' << lines[i].y_2 << ')' << endl;
		drawLines(lines[i].x_1, lines[i].y_1, lines[i].x_2, lines[i].y_2);
	}
	
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = 0; j < polygons[i].size();j++)
			drawLines(polygons[i][j].x_1, polygons[i][j].y_1, polygons[i][j].x_2, polygons[i][j].y_2);
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
			{
				cout << "df" << endl;
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
				{
					lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_2;
					lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_2;
					left_button_down = 1;

					system_state = LINE_STATE2;
				}
				else if ((abs(x - lines[lines.size() - 1].x_2) < 10) && (abs(CurrentHeight - y - lines[lines.size() - 1].y_2) < 10))
				{
					left_button_down = 1;

					system_state = LINE_STATE2;
				}
				else
				{
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
			}
		}
		break;

	case POLYGON_STATE1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
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
//				glutPostRedisplay();
				system_state = POLYGON_STATE3;
			}
		}
		break;
	case POLYGON_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				cout << x << "," << CurrentHeight - y << endl;
				cout << polygons[polygons.size() - 1][0].x_1 << "," << polygons[polygons.size() - 1][0].y_1 << endl;

				if ((abs(x - polygons[polygons.size() - 1][0].x_1) < 5) && (abs(CurrentHeight - y - polygons[polygons.size() - 1][0].y_1) < 5))
				{
					cout << "qq" << endl;
					system_state = POLYGON_STATE4;
					left_button_up = 0;
					polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = polygons[polygons.size() - 1][0].x_1;
					polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = polygons[polygons.size() - 1][0].y_1;
				}
				else
				{
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
			{
				//start to edit
				system_state = POLYGON_STATE5;
			}
		}
		break;
	case POLYGON_STATE5: //edit polygon state
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
			else if (state == GLUT_DOWN)
			{
				int i = 0;
				for (; i < polygons[polygons.size() - 1].size(); i++)
				{
					if ((abs(x - polygons[polygons.size() - 1][i].x_1) < 10) && (abs(CurrentHeight - y - polygons[polygons.size() - 1][i].y_1) < 10))
						break;
				}
				if (i < polygons[polygons.size() - 1].size())
				{

					cout << "asd" << endl;
					left_button_down = 1;

					//(i - 1)th and ith edge need to be edited;
					edit_polygon_point = i;
					system_state = POLYGON_STATE5;
				}
				else
				{
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
		}
		break;
	default: break;
	}
}

void myMotion(int x, int y)
{
	if ((system_state == LINE_STATE1) || (system_state == LINE_STATE2) || (system_state == LINE_STATE3))
	{
		if (left_button_down == 1)
		{
			lines[lines.size() - 1].x_2 = x;
			lines[lines.size() - 1].y_2 = CurrentHeight - y;
		}
	}
	else if ((system_state == POLYGON_STATE1) || (system_state == POLYGON_STATE2) || (system_state == POLYGON_STATE3))
	{
		if (left_button_down == 1)
		{
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2 = x;
			polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2 = CurrentHeight - y;
		}
	}
	else if (system_state == POLYGON_STATE5)
	{
		if (left_button_down == 1)
		{
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
	glutPostRedisplay();
}

void myPassiveMotion(int x, int y)
{
	if ((system_state == POLYGON_STATE2) || (system_state == POLYGON_STATE3))
	{
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
	default: break;
	}
}

void createPopupMenus() {

	int mainMenu = glutCreateMenu(mainMenuProc);

	glutAddMenuEntry("Draw lines", LINE);
	glutAddMenuEntry("Draw circles", CIRCLE);
	glutAddMenuEntry("Draw polygons", POLYGON);

	//fillMenu = glutCreateMenu(processFillMenu);

	//glutAddMenuEntry("Fill", FILL);
	//glutAddMenuEntry("Line", LINE);

	//colorMenu = glutCreateMenu(processColorMenu);
	//glutAddMenuEntry("Red", RED);
	//glutAddMenuEntry("Blue", BLUE);
	//glutAddMenuEntry("Green", GREEN);
	//glutAddMenuEntry("Orange", ORANGE);

	//mainMenu = glutCreateMenu(processMainMenu);

	//glutAddSubMenu("Polygon Mode", fillMenu);
	//glutAddSubMenu("Color", colorMenu);
	
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitialWidth, InitialHeight);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

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




