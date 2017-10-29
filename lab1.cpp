//#include <iostream>
//#include <stdlib.h>
//#include <vector>
//#include <GL/glut.h>
//using namespace std;

//
//int mouse_state = 0;
//float line_start_x = 0;
//float line_start_y = 0;
//float line_end_x = 0;
//float line_end_y = 0;
//
//void draw_line(float start_x, float start_y, float end_x, float end_y)
//{
//	int x1 = start_x;
//	int y1 = start_y;
//	int x2 = end_x;
//	int y2 = end_y;
//
//	glColor3f(1, 0, 0);
//	glBegin(GL_POINTS);
//	if ((start_x == end_x) && (start_y == end_y))
//	{
//		return;
//	}
////	glVertex2f(x, 400 - y);
//
//	//int dy = abs(y2 - y1);
//	//int dx = abs(x2 - x1);
//	//if (dx >= dy) {
//	//	int incy = (dy != 0 ? dy / (y2 - y1) : 0);
//	//	int p = 2 * dy - dx;
//	//	int dy2 = 2 * dy;
//	//	int dd2 = 2 * (dy - dx);
//	//	int x, y = y1;
//	//	for (x = x1 + 1; x <= x2; ++x) {
//	//		if (p<0) {
//	//			glVertex2i(x, y);
//	//			p += dy2;
//	//		}
//	//		else {
//	//			y += incy;
//	//			glVertex2i(x, y);
//	//			p += dd2;
//	//		}
//	//	}
//
//	//}
//
//	//else {
//	//	//if (y1>y2) {
//	//	//	swap(x1, x2);
//	//	//	swap(y1, y2);
//	//	//}
//	//	int incx = (dx != 0 ? dx / (x2 - x1) : 0);
//	//	int p = 2 * dx - dy;
//	//	int dx2 = 2 * dx;
//	//	int dd2 = 2 * (dx - dy);
//	//	int x = x1, y;
//	//	for (y = y1 + 1; y <= y2; ++y) {
//	//		if (p<0) {
//	//			glVertex2i(x, y);
//	//			p += dx2;
//	//		}
//	//		else {
//	//			x += incx;
//	//			glVertex2i(x, y);
//	//			p += dd2;
//	//		}
//	//	}
//	//}
//
//	glEnd();
//
//	glFlush();
//}
//
//
//void InitEnvironment()
//{
//	glClearColor(0.5, 0.5, 0.5, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	glPointSize(10);
////	glMatrixMode(GL_MODELVIEW);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, 400, 0, 400);
//}
//
//void process_menu(int option)
//{
//	switch (option)
//	{
//	case 1:
//		break;
//	default:
//		break;
//	}
//}
//
//void create_menu()
//{
//
//
//
//	int submenu = glutCreateMenu(process_menu);
//	glutAddMenuEntry("1", 1);
//
//
//	int menu = glutCreateMenu(process_menu);
//	glutAddMenuEntry("1", 1);
//
//
//	glutAddSubMenu("2", submenu);
//	
//	
//	glutAttachMenu(GLUT_RIGHT_BUTTON);
//}
//
//
//void myDisplay(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glFlush();
//}
//
//void OnMouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN)
//	{
//		if (mouse_state == 0)
//		{
//			glColor3f(1, 0, 0);
//			glBegin(GL_POINTS);
//			glVertex2f(x, 400 - y);
//			glEnd();
//			glFlush();
//
//			line_start_x = x;
//			line_start_y = 400 - y;
//
//			mouse_state = 1;
//		}
//		else if (mouse_state == 1)
//		{
//			glColor3f(1, 0, 0);
//			glBegin(GL_POINTS);
//			glVertex2f(x, 400 - y);
//			glEnd();
//			glFlush();
//
//			line_end_x = x;
//			line_end_y = 400 - y;
//
//			draw_line(line_start_x, line_start_y, line_end_x, line_end_y);
//
//			mouse_state = 0;
//		}
//	}
//	if (button == GLUT_MIDDLE_BUTTON&&state == GLUT_DOWN)
//	{
//		glColor3f(0, 1, 0);
//		glBegin(GL_POINTS);
//		glVertex2f(x, 400 - y);
//		glEnd();
//		glFlush();
//	}
//	if (button == GLUT_RIGHT_BUTTON&&state == GLUT_DOWN)
//	{
//		glColor3f(0, 0, 1);
//		glBegin(GL_POINTS);
//		glVertex2f(x, 400 - y);
//		glEnd();
//		glFlush();
//	}
//}
//
//void myMotion(int x, int y)
//{
//	if (mouse_state == 1)
//	{
//		;
//	}
//}
//
//void myReshape(GLint w, GLint h) {
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0, w, 0.0, h, -1, 1);
//	glMatrixMode(GL_MODELVIEW);
//}
//
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);   //初始化GLUT  
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(400,400);
//	glutCreateWindow("OpenGL");
//	
//	
//	InitEnvironment();   //初始化  
//
//
//	create_menu();
//
//	glutMouseFunc(OnMouse);  //注册鼠标事件  
//	
//	glutDisplayFunc(myDisplay);   //回调函数   
//	
//	glutMotionFunc(myMotion);
//
//	glutReshapeFunc(myReshape);
//
//
//	glutMainLoop();    //持续显示，当窗口改变会重新绘制图形  
//	return 0;
//}


//glutSetWindow(mainWindow);
//glutPostRedisplay();

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
using namespace std;

#define InitialWidth 320
#define InitialHeight 320
#define LINE 1
#define LINE_STATE1 101
#define LINE_MAX_NUM 10
int system_state = 0;
float CurrentWidth = InitialWidth;
float CurrentHeight = InitialHeight;
int left_button_down = 0;
int line_num = 0;

struct Line
{
	int x_1;
	int y_1;
	int x_2;
	int y_2;
};
vector<Line> lines;

void drawLines(int x_1, int y_1, int x_2, int y_2)
{
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	
//	glVertex2i(x_1, x_2);

	int x, y, dx, dy, p;
	x = x_1;
	y = y_1;
	dx = x_2 - x_1;
	dy = y_2 - y_1;
	p = 2 * dy - dx;

	for (; x <= x_2; x++)
	{
		glVertex2i(x, y);
		if (p >= 0)
		{
			y++;
			p += 2 * (dy - dx);
		}
		else
		{
			p += 2 * dy;
		}
	}

	glEnd();

	return;
}

void InitEnvironment()
{
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(5);
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

//	float r = 1.0 * CurrentWidth / CurrentHeight;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
//	if (h == 0)
//		h = 1;

//	float ratio = w * 1.0 / h;

	CurrentWidth = w;
	CurrentHeight = h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
//	glViewport(0, 0, InitialWidth, InitialHeight);
//	glViewport(0, 0, r * CurrentHeight, CurrentHeight);

	// Set the correct perspective.
//	gluPerspective(45, ratio, 1, 100);
//	gluOrtho2D(0, InitialWidth, 0, InitialHeight);
//	gluOrtho2D(0, r * CurrentHeight, 0, CurrentHeight);

	glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH) * 2, glutGet(GLUT_SCREEN_HEIGHT) * 2);
	gluOrtho2D(0, glutGet(GLUT_SCREEN_WIDTH) * 2, 0, glutGet(GLUT_SCREEN_HEIGHT) * 2);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColor3d(0, 0, 0);
	//glBegin(GL_TRIANGLES);
	//glVertex2f(0, 100);
	//glVertex2f(100, 0);
	//glVertex2f(50, 100);
	//glEnd();
	//glFlush();

	//glColor3f(1, 0, 0);
	//glBegin(GL_POINTS);
	//glVertex2f(200, 200);
	//glEnd();

	for (int i = 0; i < lines.size(); i++)
		drawLines(lines[i].x_1, lines[i].y_1, lines[i].x_2, lines[i].y_2);


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
				left_button_down = 1;

				Line newLine;
				newLine.x_1 = x;
				newLine.y_1 = CurrentHeight - y;
				newLine.x_2 = x;
				newLine.y_2 = CurrentHeight - y;
				lines.push_back(newLine);

/*				glColor3f(0, 0, 0);
				glBegin(GL_POINTS);
				glVertex2f(x, CurrentHeight - y);
				glEnd();

				glFlush();
*/
				glutPostRedisplay();
			}
			else
			{
				left_button_down = 0;
			}
		}
		break;
	deafult: break;
	}
}

void myMotion(int x, int y)
{
	if (left_button_down == 1)
	{
		lines[lines.size() - 1].x_2 = x;
		lines[lines.size() - 1].y_2 = CurrentHeight - y;

	}
	
	glutPostRedisplay();

}

void mainMenuProc(int option) {

	switch (option) {

	case LINE: 
		system_state = LINE_STATE1;
		break;
	default: break;
	}
}

void createPopupMenus() {

	int mainMenu = glutCreateMenu(mainMenuProc);

	glutAddMenuEntry("Draw a line", LINE);
//	glutAddMenuEntry("NORMAL", NORMAL);

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
//	glutInitWindowPosition(100, 100);
	glutInitWindowPosition(0, 0);

//	int w = glutGet(GLUT_SCREEN_WIDTH);
//	int h = glutGet(GLUT_SCREEN_HEIGHT);
//	glutInitWindowSize(w, h);

	glutInitWindowSize(InitialWidth, InitialHeight);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	

	InitEnvironment();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(myMotion);

	// init Menus
	createPopupMenus();

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}

//#include <assert.h>
//#include <cmath>
//#include <iostream>
//#include <vector>
//#include <GL/glut.h>
//using namespace std;
//
//typedef vector<int> list;
//vector<list> ls, cs;
//int old_x, old_y, new_x, new_y, cx, cy, cr;
//int mouse_type;
//
///* breseman画线算法 */
//void line(int x1, int y1, int x2, int y2) {
//	glBegin(GL_POINTS);
//	if (x1 == x2 && y1 == y2) {
//		glVertex2i(x1, y1);
//		return;
//	}
//	if (x1>x2) {
//		swap(x1, x2);
//		swap(y1, y2);
//	}
//	glVertex2d(x1, y1);
//	int dy = abs(y2 - y1);
//	int dx = abs(x2 - x1);
//	if (dx >= dy) {
//		int incy = (dy != 0 ? dy / (y2 - y1) : 0);
//		int p = 2 * dy - dx;
//		int dy2 = 2 * dy;
//		int dd2 = 2 * (dy - dx);
//		int x, y = y1;
//		for (x = x1 + 1; x <= x2; ++x) {
//			if (p<0) {
//				glVertex2i(x, y);
//				p += dy2;
//			}
//			else {
//				y += incy;
//				glVertex2i(x, y);
//				p += dd2;
//			}
//		}
//	}
//	else {
//		if (y1>y2) {
//			swap(x1, x2);
//			swap(y1, y2);
//		}
//		int incx = (dx != 0 ? dx / (x2 - x1) : 0);
//		int p = 2 * dx - dy;
//		int dx2 = 2 * dx;
//		int dd2 = 2 * (dx - dy);
//		int x = x1, y;
//		for (y = y1 + 1; y <= y2; ++y) {
//			if (p<0) {
//				glVertex2i(x, y);
//				p += dx2;
//			}
//			else {
//				x += incx;
//				glVertex2i(x, y);
//				p += dd2;
//			}
//		}
//	}
//	glEnd();
//}
//
//
///* 中点圆算法 */
//void circle(int cx, int cy, int r) {
//	int x = 0, y = r;
//	float p = 1.25 - r;
//	while (x <= y) {
//		glBegin(GL_POINTS);
//		glVertex2i(cx + x, cy + y);
//		glVertex2i(cx - x, cy + y);
//		glVertex2i(cx - x, cy - y);
//		glVertex2i(cx + x, cy - y);
//		glVertex2i(cx + y, cy + x);
//		glVertex2i(cx - y, cy + x);
//		glVertex2i(cx - y, cy - x);
//		glVertex2i(cx + y, cy - x);
//		glEnd();
//		if (p<0) {
//			++x;
//			p += 2 * x + 1;
//		}
//		else {
//			p += 2 * x - 2 * y + 5;
//			++x;
//			--y;
//		}
//	}
//}
//
//
//void draw(){
//	int i;
//	for (i = 0; i<ls.size(); ++i)
//		line(ls[i][0], ls[i][1], ls[i][2], ls[i][3]);
//	for (i = 0; i<cs.size(); ++i)
//		circle(cs[i][0], cs[i][1], cs[i][2]);
//	if (mouse_type == 1)
//		line(old_x, old_y, new_x, new_y);
//	if (mouse_type == 3)
//		circle(cx, cy, cr); //圆上的点和半径
//}
//
//void display() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	glColor3i(0, 0, 0);
//	draw();
//	glutSwapBuffers();
//}
//
//void keyboard(unsigned char key, int x, int y) {
//	switch (key) {
//	case 27:
//		exit(0); break;
//	case 'a':
//	case 'A':
//		break;
//	default:
//		if (key <= 'z' && key >= 'a' || key <= 'Z' && key >= 'A' || key <= '9' && key >= '0'){
//			cout << char(key);
//		}
//		else{
//			cout << "\n字符码为 " << static_cast<int>(key);
//			display();
//		}
//	}
//}
//
//
//void mouseclick(int button, int state, int x, int y){
//	cout << "(" << x << "," << y << ")";
//	switch (button) {
//	case GLUT_LEFT_BUTTON:
//		if (state == GLUT_DOWN) {
//			cout << "左键被按下";
//			old_x = x;
//			old_y = 600 - y;
//			mouse_type = 1;
//		}
//		else if (state == GLUT_UP) {
//			cout << "左键被松开";
//			list l;
//			l.push_back(old_x);
//			l.push_back(old_y);
//			l.push_back(x);
//			l.push_back(600 - y);
//			ls.push_back(l);
//			mouse_type = 0;
//			glutPostRedisplay();
//			for (int i = 0; i<ls.size(); ++i)
//				cout << endl << "line_" << i << ": " << ls[i][0] << "," << ls[i][1] << "   " << ls[i][2] << "," << ls[i][3];
//		}
//		break;
//	case GLUT_MIDDLE_BUTTON:
//		break;
//	case GLUT_RIGHT_BUTTON:
//		if (state == GLUT_DOWN) {
//			cout << "右键被按下";
//			cx = x;
//			cy = 600 - y;
//			mouse_type = 3;
//		}
//		else if (state == GLUT_UP) {
//			cout << "右键被松开";
//			list l;
//			l.push_back(cx);
//			l.push_back(cy);
//			cr = int(sqrt(double(x - cx)*(x - cx) + (600 - y - cy)*(600 - y - cy)) + 0.5);
//			l.push_back(cr);
//			cs.push_back(l);
//			mouse_type = 2;
//			glutPostRedisplay();
//			for (int i = 0; i<cs.size(); ++i)
//				cout << endl << "circle_" << i << ": " << cs[i][0] << "," << cs[i][1] << "   " << cs[i][2];
//		}
//		break;
//	}
//}
//
//void screenmotion(int x, int y){
//	if (mouse_type == 1){
//		new_x = x;
//		new_y = 600 - y;
//		cout << endl << "new-line:" << old_x << "," << old_y << "   " << new_x << "," << new_y << endl;
//	}
//	else if (mouse_type == 3){
//		cr = int(sqrt(double(x - cx)*(x - cx) + (600 - y - cy)*(600 - y - cy)) + 0.5);
//		cout << endl << "new-circle: " << cx << "," << cy << "   " << cr << endl;
//	}
//
//	glutPostRedisplay();
//
//}
//
//void init() {
//	glClearColor(0.2, 0.5, 0.8, 0.0); //背景颜色
//	glMatrixMode(GL_PROJECTION);
//	gluOrtho2D(-1.0, 600, 0.0, 601);
//}
//
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowPosition(100, 50); //图框在屏幕的位置
//	glutInitWindowSize(600, 600);  //边框大小
//	glutCreateWindow("图元算法");  //图框边显示的字
//	init();
//	glutDisplayFunc(display);
//	glutMotionFunc(screenmotion);
//	glutKeyboardFunc(keyboard);
//	glutMouseFunc(mouseclick);
//	glutMainLoop();
//	return 0;
//}
//



