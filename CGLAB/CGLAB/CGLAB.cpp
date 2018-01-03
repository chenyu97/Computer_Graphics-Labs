#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <GL/glut.h>
using namespace std;

#define BMP_Header_Length 54
#define InitialWidth glutGet(GLUT_SCREEN_WIDTH)
#define InitialHeight glutGet(GLUT_SCREEN_HEIGHT)
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
#define POLYGON_STATE_CUT1 601
#define POLYGON_STATE_CUT2 602
#define POLYGON_STATE_CUT3 603
#define POLYGON_STATE_CUT4 604
#define POLYGON_STATE_CUT5 605
#define POLYGON_STATE_CUT6 606
#define CURVE 7
#define CURVE_STATE1 701
#define CURVE_STATE2 702
#define CURVE_STATE3 703
#define CURVE_STATE4 704
#define CURVE_STATE5 705
#define CURVE_STATE6 706
#define CURVE_STATE7 707
#define CURVE_STATE8 708
#define CURVE_STATE9 709
#define CURVE_STATE10 710
#define CUBE 8
#define CUBE_STATE1 801

int system_state = 0;
float CurrentWidth = InitialWidth;
float CurrentHeight = InitialHeight;
int left_button_down = 0;
int left_button_up = 0;
int edit_line_point = -1;
int edit_polygon_point = -1;
int edit_filledArea_point = -1;
int edit_curve_point = -1;
int isFilledAreaEdit = 0;
int isFilledAreaEnd = 0;
int isCircleEdit = 0;
int isEllipseEdit = 0;
int isCurveDraw = 0;
int resizeLine = 0;
int resizePolygon = 0;
int resizeFilledArea = 0;
int resizeCircle = 0;
int resizeEllipse = 0;
int resizeCurve = 0;
float angle = 0;

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

struct Circle
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

struct Curve
{
	floatPoint p1;
	floatPoint p2;
	floatPoint p3;
	floatPoint p4;
};

vector<list<Node>> ET;
list<Node> AET;

vector<Line> lines;
vector<vector<Line>> polygons;
vector<Ellipse> ellipses;
vector<Circle> circles;
vector<vector<Line>> filledAreas;
vector<Curve> curves;

Line circleBounds[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };  //to surround the current circle
Line ellipseBounds[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };  //to surround the current ellipse
Line cutBounds[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }; //the cutting window

static GLubyte BMP_Header[BMP_Header_Length] = {
	0x42, //0  
	0x4d, //1  
	0x66, //2  
	0x75, //3  
	0x00, //4  
	0x00, //5  
	0x00, //6  
	0x00, //7  
	0x00, //8  
	0x00, //9  
	0x36, //a  
	0x00, //b  
	0x00, //c  
	0x00, //d  
	0x28, //e  
	0x00,//f  
	0x00, //0  
	0x00, //1  
	0x64, //2  
	0x00, //3  
	0x00, //4  
	0x00, //5  
	0x64, //6  
	0x00, //7  
	0x00, //8  
	0x00, //9  
	0x01, //a  
	0x00, //b  
	0x18, //c  
	0x00, //d  
	0x00, //e  
	0x00,//f  
	0x00, //0  
	0x00, //1  
	0x30, //2  
	0x75//3  
};

void rotate3D()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(angle, -1.0f , 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f , 1.0f, -1.0f);
	glVertex3f(-1.0f , 1.0f, -1.0f);
	glVertex3f(-1.0f , 1.0f, 1.0f);
	glVertex3f(1.0f , 1.0f, 1.0f);
	
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	
	glEnd();
	angle -= 1.0f;
	return;
}

void drawBezierCurve(Curve curve)
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i <= 1000; i++)
	{
		GLfloat t = i / 1000.0;
		GLfloat b0 = pow(1 - t, 3.0);
		GLfloat b1 = 3.0 * t * pow(1 - t, 2.0);
		GLfloat b2 = 3.0 * t * t * (1 - t);
		GLfloat b3 = t * t * t;

		GLfloat x = curve.p1.x_1 * b0 + curve.p2.x_1 * b1 + curve.p3.x_1 * b2 + curve.p4.x_1 * b3;
		GLfloat y = curve.p1.y_1 * b0 + curve.p2.y_1 * b1 + curve.p3.y_1 * b2 + curve.p4.y_1 * b3;
		glVertex2f(x, y);
	}
	glEnd();
	return;
}

void grab()
{
	//	FILE*    pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据
	FILE*    pWritingFile;  //指向要保存截图的bmp文件
	GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据
	//	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;   //BMP文件数据总长度

	// 计算像素数据的实际长度
	i = CurrentWidth * 3;   // 得到每一行的像素数据长度
	while (i % 4 != 0)      // 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * CurrentHeight;  //补齐后的总位数

	// 分配内存和打开文件
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	//	pDummyFile = fopen("bitmap1.bmp", "rb");//只读形式打开
	//	if (pDummyFile == 0)
	//		exit(0);

	pWritingFile = fopen("grab.bmp", "wb"); //只写形式打开
	if (pWritingFile == 0)
		exit(0);

	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	//	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存
	i = CurrentWidth;
	j = CurrentHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式
	glReadPixels(0, 0, CurrentWidth, CurrentHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	//把完整的BMP文件数据写入pWritingFile
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	//	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

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

bool isOutside(Line cutLine, floatPoint p)
{
	if (((p.y_1 - cutLine.y_1) * (cutLine.x_2 - cutLine.x_1) - (cutLine.y_2 - cutLine.y_1) * (p.x_1 - cutLine.x_1)) > 0)
		return 1;
	return 0;
}

void polygonCut()
{
	float xMin = cutBounds[0].x_1;
	for (int i = 0; i < 4; i++)
	{
		if (cutBounds[i].x_1 < xMin)
			xMin = cutBounds[i].x_1;
		else if (cutBounds[i].x_2 < xMin)
			xMin = cutBounds[i].x_2;
	}

	float xMax = cutBounds[0].x_1;
	for (int i = 0; i < 4; i++)
	{
		if (cutBounds[i].x_1 > xMax)
			xMax = cutBounds[i].x_1;
		else if (cutBounds[i].x_2 > xMax)
			xMax = cutBounds[i].x_2;
	}

	float yMin = cutBounds[0].y_1;
	for (int i = 0; i < 4; i++)
	{
		if (cutBounds[i].y_1 < yMin)
			yMin = cutBounds[i].y_1;
		else if (cutBounds[i].y_2 < yMin)
			yMin = cutBounds[i].y_2;
	}

	float yMax = cutBounds[0].y_1;
	for (int i = 0; i < 4; i++)
	{
		if (cutBounds[i].y_1 > yMax)
			yMax = cutBounds[i].y_1;
		else if (cutBounds[i].y_2 > yMax)
			yMax = cutBounds[i].y_2;
	}

	Line cutWindowBounds[4] = { { xMin, yMin, xMin, yMax }, { xMin, yMax, xMax, yMax }, { xMax, yMax, xMax, yMin }, { xMax, yMin, xMin, yMin } };

	floatPoint peStart, peStop;	//polygon edge Start/Stop
	floatPoint cweStart, cweStop;	//cutting window edge Start/Stop
	bool lpii = 0;	//last point is inside

	cweStart.x_1 = xMax;
	cweStart.y_1 = yMin;

	vector<floatPoint> newPoints;
	vector<floatPoint> tempPolygon;
	for (int i = 0; i < polygons[polygons.size() - 1].size(); i++)
	{
		floatPoint tp;
		tp.x_1 = polygons[polygons.size() - 1][i].x_1;
		tp.y_1 = polygons[polygons.size() - 1][i].y_1;
		tempPolygon.push_back(tp);
	}

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			cweStop.x_1 = xMin;
			cweStop.y_1 = yMin;
		}
		else if (i == 1)
		{
			cweStop.x_1 = xMin;
			cweStop.y_1 = yMax;
		}
		else if (i == 2)
		{
			cweStop.x_1 = xMax;
			cweStop.y_1 = yMax;
		}
		else if (i == 3)
		{
			cweStop.x_1 = xMax;
			cweStop.y_1 = yMin;
		}

		if (tempPolygon.size() == 0)
		{
			polygons.erase(polygons.end() - 1);
			return;
		}
		else if (tempPolygon.size() == 1)
		{
			if (i == 0)
			{
				if (tempPolygon[0].y_1 < yMin)
					return;
				else
					newPoints.push_back(tempPolygon[0]);
			}
			else if (i == 1)
			{
				if (tempPolygon[0].x_1 < xMin)
					return;
				else
					newPoints.push_back(tempPolygon[0]);
			}
			else if (i == 2)
			{
				if (tempPolygon[0].y_1 > yMax)
					return;
				else
					newPoints.push_back(tempPolygon[0]);
			}
			else if (i == 3)
			{
				if (tempPolygon[0].x_1 > xMax)
					return;
				else
					newPoints.push_back(tempPolygon[0]);
			}
		}
		else
		{
			peStart.x_1 = tempPolygon[tempPolygon.size() - 1].x_1;
			peStart.y_1 = tempPolygon[tempPolygon.size() - 1].y_1;

			Line lineTemp;
			lineTemp.x_1 = cweStart.x_1;
			lineTemp.y_1 = cweStart.y_1;
			lineTemp.x_2 = cweStop.x_1;
			lineTemp.y_2 = cweStop.y_1;
			lpii = !(isOutside(lineTemp, peStart));

			for (int j = 0; j < tempPolygon.size(); j++)
			{
				peStop.x_1 = tempPolygon[j].x_1;
				peStop.y_1 = tempPolygon[j].y_1;

				lineTemp.x_1 = cweStart.x_1;
				lineTemp.y_1 = cweStart.y_1;
				lineTemp.x_2 = cweStop.x_1;
				lineTemp.y_2 = cweStop.y_1;
				if (!(isOutside(lineTemp, peStop)))
				{
					if (!lpii)
					{
						lpii = 1;
						//calculate the new point
						floatPoint newPoint;
						if (i == 0)
						{
							newPoint.y_1 = yMin;
							newPoint.x_1 = peStart.x_1 + (newPoint.y_1 - peStart.y_1) * (peStop.x_1 - peStart.x_1) / (peStop.y_1 - peStart.y_1);
						}
						else if (i == 1)
						{
							newPoint.x_1 = xMin;
							newPoint.y_1 = (newPoint.x_1 - peStart.x_1) * (peStop.y_1 - peStart.y_1) / (peStop.x_1 - peStart.x_1) + peStart.y_1;
						}
						else if (i == 2)
						{
							newPoint.y_1 = yMax;
							newPoint.x_1 = peStart.x_1 + (newPoint.y_1 - peStart.y_1) * (peStop.x_1 - peStart.x_1) / (peStop.y_1 - peStart.y_1);

						}
						else if (i == 3)
						{
							newPoint.x_1 = xMax;
							newPoint.y_1 = (newPoint.x_1 - peStart.x_1) * (peStop.y_1 - peStart.y_1) / (peStop.x_1 - peStart.x_1) + peStart.y_1;
						}
						newPoints.push_back(newPoint);
					}
					newPoints.push_back(peStop);
				}
				else
				{
					if (lpii)
					{
						lpii = 0;

						floatPoint newPoint;
						if (i == 0)
						{
							newPoint.y_1 = yMin;
							newPoint.x_1 = peStart.x_1 + (newPoint.y_1 - peStart.y_1) * (peStop.x_1 - peStart.x_1) / (peStop.y_1 - peStart.y_1);
						}
						else if (i == 1)
						{
							newPoint.x_1 = xMin;
							newPoint.y_1 = (newPoint.x_1 - peStart.x_1) * (peStop.y_1 - peStart.y_1) / (peStop.x_1 - peStart.x_1) + peStart.y_1;
						}
						else if (i == 2)
						{
							newPoint.y_1 = yMax;
							newPoint.x_1 = peStart.x_1 + (newPoint.y_1 - peStart.y_1) * (peStop.x_1 - peStart.x_1) / (peStop.y_1 - peStart.y_1);

						}
						else if (i == 3)
						{
							newPoint.x_1 = xMax;
							newPoint.y_1 = (newPoint.x_1 - peStart.x_1) * (peStop.y_1 - peStart.y_1) / (peStop.x_1 - peStart.x_1) + peStart.y_1;
						}
						newPoints.push_back(newPoint);
					}
				}
				peStart.x_1 = peStop.x_1;
				peStart.y_1 = peStop.y_1;
			}
		}
		cweStart.x_1 = cweStop.x_1;
		cweStart.y_1 = cweStop.y_1;
		tempPolygon.swap(newPoints);
		newPoints.clear();
	}

	if (tempPolygon.size() == 0)
		polygons.erase(polygons.end() - 1);
	else
	{
		polygons.erase(polygons.end() - 1);
		vector<Line> polygonAdd;
		for (int i = 0; i < tempPolygon.size(); i++)
		{
			Line lineAdd;
			lineAdd.x_1 = tempPolygon[i].x_1;
			lineAdd.y_1 = tempPolygon[i].y_1;
			lineAdd.x_2 = tempPolygon[(i + 1) % (tempPolygon.size())].x_1;
			lineAdd.y_2 = tempPolygon[(i + 1) % (tempPolygon.size())].y_1;
			polygonAdd.push_back(lineAdd);
		}
		polygons.push_back(polygonAdd);
	}
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

	//ADD
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);  
}

void changeSize(int w, int h) {
	if (system_state == CUBE_STATE1)
	{
		glViewport(0, 0, glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, glutGet(GLUT_SCREEN_WIDTH) / glutGet(GLUT_SCREEN_HEIGHT), 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -10.0f);
	}
	else
	{
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
		
}

void renderScene(void) {
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (system_state == CUBE_STATE1){
		rotate3D();
	}
	else
	{
		glLoadIdentity();
		//draw lines
		for (int i = 0; i < lines.size(); i++)
		{
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

		//draw curves
		for (int i = 0; i < curves.size(); i++)
		{
			if (i == curves.size() - 1 && ((system_state / 100) == 7) && (system_state != CURVE_STATE1))
			{
				drawLines(curves[i].p1.x_1, curves[i].p1.y_1, curves[i].p2.x_1, curves[i].p2.y_1);
				drawLines(curves[i].p2.x_1, curves[i].p2.y_1, curves[i].p3.x_1, curves[i].p3.y_1);
				drawLines(curves[i].p3.x_1, curves[i].p3.y_1, curves[i].p4.x_1, curves[i].p4.y_1);
			}
			if (i != curves.size() - 1 || ((i == curves.size() - 1) && (isCurveDraw == 0)))
			{
				drawBezierCurve(curves[i]);
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

		//draw cutBounds
		if (((system_state / 100) == 6) && (system_state != POLYGON_STATE_CUT1))
		{
			for (int i = 0; i < 4; i++)
			{
				drawLines(cutBounds[i].x_1, cutBounds[i].y_1, cutBounds[i].x_2, cutBounds[i].y_2);
			}
		}

	}
	//	glFlush();
	glutSwapBuffers();
	grab();

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
		break;
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
	case CURVE_STATE1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				isCurveDraw = 1;
				edit_curve_point = -1;
				system_state = CURVE_STATE2;
				left_button_down = 1;
				

				Curve newCurve;
				newCurve.p1.x_1 = x;
				newCurve.p1.y_1 = CurrentHeight - y;
				newCurve.p2.x_1 = x;
				newCurve.p2.y_1 = CurrentHeight - y;
				newCurve.p3.x_1 = x;
				newCurve.p3.y_1 = CurrentHeight - y;
				newCurve.p4.x_1 = x;
				newCurve.p4.y_1 = CurrentHeight - y;
				curves.push_back(newCurve);

				glutPostRedisplay();
			}
		}
		break;
	case CURVE_STATE2:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_up = 1;
				left_button_down = 0;
				system_state = CURVE_STATE3;
			}
		}
		break;
	case CURVE_STATE3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				system_state = CURVE_STATE4;
				left_button_down = 1;
				left_button_up = 0;
				
				curves[curves.size() - 1].p2.x_1 = x;
				curves[curves.size() - 1].p2.y_1 = CurrentHeight - y;
				curves[curves.size() - 1].p3.x_1 = x;
				curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
				curves[curves.size() - 1].p4.x_1 = x;
				curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;

				glutPostRedisplay();
			}
		}
		break;
	case CURVE_STATE4:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_up = 1;
				left_button_down = 0;
				system_state = CURVE_STATE5;
			}
		}
		break;
	case CURVE_STATE5:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				system_state = CURVE_STATE6;
				left_button_down = 1;
				left_button_up = 0;

				curves[curves.size() - 1].p3.x_1 = x;
				curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
				curves[curves.size() - 1].p4.x_1 = x;
				curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;

				glutPostRedisplay();
			}
		}
		break;
	case CURVE_STATE6:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_up = 1;
				left_button_down = 0;
				system_state = CURVE_STATE7;
			}
		}
		break;
	case CURVE_STATE7:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				system_state = CURVE_STATE8;
				left_button_down = 1;
				left_button_up = 0;

				curves[curves.size() - 1].p4.x_1 = x;
				curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;

				isCurveDraw = 0;

				glutPostRedisplay();
			}
		}
		break;
	case CURVE_STATE8:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{	//start to edit
				system_state = CURVE_STATE9;
			}
		}
		break;
	case CURVE_STATE9:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
			else if (state == GLUT_DOWN)
			{	//to edit
				if ((abs(x - curves[curves.size() - 1].p4.x_1) < 10) && (abs(CurrentHeight - y - curves[curves.size() - 1].p4.y_1) < 10))
				{
					left_button_down = 1;
					edit_curve_point = 4;
					system_state = CURVE_STATE9;
				}
				else if ((abs(x - curves[curves.size() - 1].p3.x_1) < 10) && (abs(CurrentHeight - y - curves[curves.size() - 1].p3.y_1) < 10))
				{
					left_button_down = 1;
					edit_curve_point = 3;
					system_state = CURVE_STATE9;
				}
				else if ((abs(x - curves[curves.size() - 1].p2.x_1) < 10) && (abs(CurrentHeight - y - curves[curves.size() - 1].p2.y_1) < 10))
				{
					left_button_down = 1;
					edit_curve_point = 2;
					system_state = CURVE_STATE9;
				}
				else if ((abs(x - curves[curves.size() - 1].p1.x_1) < 10) && (abs(CurrentHeight - y - curves[curves.size() - 1].p1.y_1) < 10))
				{
					left_button_down = 1;
					edit_curve_point = 1;
					system_state = CURVE_STATE9;
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
					edit_curve_point = -1;
					 
					system_state = CURVE_STATE1;
				}
			}
		}
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
	case POLYGON_STATE_CUT1:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				left_button_down = 1;

				cutBounds[0].x_1 = x;
				cutBounds[0].y_1 = CurrentHeight - y;
				cutBounds[0].x_2 = x;
				cutBounds[0].y_2 = CurrentHeight - y;  //no use

				cutBounds[1].x_1 = x;
				cutBounds[1].y_1 = CurrentHeight - y;  //no use
				cutBounds[1].x_2 = x;  //no use
				cutBounds[1].y_2 = CurrentHeight - y;  //no use

				cutBounds[2].x_1 = x;  //no use
				cutBounds[2].y_1 = CurrentHeight - y;  //no use
				cutBounds[2].x_2 = x;  //no use
				cutBounds[2].y_2 = CurrentHeight - y;

				cutBounds[3].x_1 = x;  //no use
				cutBounds[3].y_1 = CurrentHeight - y;
				cutBounds[3].x_2 = x;
				cutBounds[3].y_2 = CurrentHeight - y;

				system_state = POLYGON_STATE_CUT2;
				glutPostRedisplay();
			}
			else if (state == GLUT_UP)
			{
				left_button_down = 0;
			}
		}
		break;
	case POLYGON_STATE_CUT2:  //ready to edit point 2	
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = POLYGON_STATE_CUT3;
			}
		}
		break;
	case POLYGON_STATE_CUT3:
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if ((abs(x - cutBounds[2].x_1) < 10) && (abs(CurrentHeight - y - cutBounds[2].y_1) < 10))
				{
					left_button_down = 1;
					system_state = POLYGON_STATE_CUT2;
				}
				else if ((abs(x - cutBounds[0].x_1) < 10) && (abs(CurrentHeight - y - cutBounds[0].y_1) < 10))
				{
					left_button_down = 1;
					system_state = POLYGON_STATE_CUT4;
				}
				else if ((abs(x - cutBounds[1].x_1) < 10) && (abs(CurrentHeight - y - cutBounds[1].y_1) < 10))
				{
					left_button_down = 1;
					system_state = POLYGON_STATE_CUT5;
				}
				else if ((abs(x - cutBounds[3].x_1) < 10) && (abs(CurrentHeight - y - cutBounds[3].y_1) < 10))
				{
					left_button_down = 1;
					system_state = POLYGON_STATE_CUT6;
				}
			}
		}
		break;
	case POLYGON_STATE_CUT4:  //ready to edit point 0
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = POLYGON_STATE_CUT3;
			}
		}
		break;
	case POLYGON_STATE_CUT5:  //ready to edit point 1
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = POLYGON_STATE_CUT3;
			}
		}
		break;
	case POLYGON_STATE_CUT6:  //ready to edit point 3
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_UP)
			{
				left_button_down = 0;
				system_state = POLYGON_STATE_CUT3;
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
	else if (system_state == CURVE_STATE2)
	{
		if (left_button_down == 1)
		{
			curves[curves.size() - 1].p2.x_1 = x;
			curves[curves.size() - 1].p2.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p3.x_1 = x;
			curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == CURVE_STATE4)
	{
		if (left_button_down == 1)
		{
			curves[curves.size() - 1].p3.x_1 = x;
			curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == CURVE_STATE6)
	{
		if (left_button_down == 1)
		{
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == CURVE_STATE9)
	{
		if (left_button_down == 1)
		{	//drag one vertex of the control points of the curve
			if (edit_curve_point == 1)
			{
				curves[curves.size() - 1].p1.x_1 = x;
				curves[curves.size() - 1].p1.y_1 = CurrentHeight - y;
			}
			else if (edit_curve_point == 2)
			{
				curves[curves.size() - 1].p2.x_1 = x;
				curves[curves.size() - 1].p2.y_1 = CurrentHeight - y;
			}
			else if (edit_curve_point == 3)
			{
				curves[curves.size() - 1].p3.x_1 = x;
				curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
			}
			else if (edit_curve_point == 4)
			{
				curves[curves.size() - 1].p4.x_1 = x;
				curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
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
	else if (system_state == POLYGON_STATE_CUT2)
	{
		if (left_button_down == 1)
		{
			//			cutBounds[0].x_1 = x;
			//			cutBounds[0].y_1 = CurrentHeight - y;
			//			cutBounds[0].x_2 = x;
			cutBounds[0].y_2 = CurrentHeight - y;

			//			cutBounds[1].x_1 = x;
			cutBounds[1].y_1 = CurrentHeight - y;
			cutBounds[1].x_2 = x;
			cutBounds[1].y_2 = CurrentHeight - y;

			cutBounds[2].x_1 = x;
			cutBounds[2].y_1 = CurrentHeight - y;
			cutBounds[2].x_2 = x;
			//			cutBounds[2].y_2 = CurrentHeight - y;

			cutBounds[3].x_1 = x;
			//			cutBounds[3].y_1 = CurrentHeight - y;
			//			cutBounds[3].x_2 = x;
			//			cutBounds[3].y_2 = CurrentHeight - y;
		}
	}
	else if (system_state == POLYGON_STATE_CUT4)
	{
		if (left_button_down == 1)
		{
			cutBounds[0].x_1 = x;
			cutBounds[0].y_1 = CurrentHeight - y;
			cutBounds[0].x_2 = x;
			//			cutBounds[0].y_2 = CurrentHeight - y;

			cutBounds[1].x_1 = x;
			//      	cutBounds[1].y_1 = CurrentHeight - y;
			//			cutBounds[1].x_2 = x;
			//			cutBounds[1].y_2 = CurrentHeight - y;

			//			cutBounds[2].x_1 = x;
			//			cutBounds[2].y_1 = CurrentHeight - y;
			//			cutBounds[2].x_2 = x;
			cutBounds[2].y_2 = CurrentHeight - y;

			//			cutBounds[3].x_1 = x;
			cutBounds[3].y_1 = CurrentHeight - y;
			cutBounds[3].x_2 = x;
			cutBounds[3].y_2 = CurrentHeight - y;
		}
	}
	else if (system_state == POLYGON_STATE_CUT5)
	{
		if (left_button_down == 1)
		{
			cutBounds[0].x_1 = x;
			//			cutBounds[0].y_1 = CurrentHeight - y;
			cutBounds[0].x_2 = x;
			cutBounds[0].y_2 = CurrentHeight - y;

			cutBounds[1].x_1 = x;
			cutBounds[1].y_1 = CurrentHeight - y;
			//			cutBounds[1].x_2 = x;
			cutBounds[1].y_2 = CurrentHeight - y;

			//			cutBounds[2].x_1 = x;
			cutBounds[2].y_1 = CurrentHeight - y;
			//			cutBounds[2].x_2 = x;
			//			cutBounds[2].y_2 = CurrentHeight - y;

			//			cutBounds[3].x_1 = x;
			//			cutBounds[3].y_1 = CurrentHeight - y;
			cutBounds[3].x_2 = x;
			//			cutBounds[3].y_2 = CurrentHeight - y;

		}
	}
	else if (system_state == POLYGON_STATE_CUT6)
	{
		if (left_button_down == 1)
		{
			//			cutBounds[0].x_1 = x;
			cutBounds[0].y_1 = CurrentHeight - y;
			//			cutBounds[0].x_2 = x;
			//			cutBounds[0].y_2 = CurrentHeight - y;

			//			cutBounds[1].x_1 = x;
			//			cutBounds[1].y_1 = CurrentHeight - y;
			cutBounds[1].x_2 = x;
			//			cutBounds[1].y_2 = CurrentHeight - y;

			cutBounds[2].x_1 = x;
			//			cutBounds[2].y_1 = CurrentHeight - y;
			cutBounds[2].x_2 = x;
			cutBounds[2].y_2 = CurrentHeight - y;

			cutBounds[3].x_1 = x;
			cutBounds[3].y_1 = CurrentHeight - y;
			//			cutBounds[3].x_2 = x;
			cutBounds[3].y_2 = CurrentHeight - y;
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
	else if (system_state == CURVE_STATE3)
	{	
		if (left_button_up == 1)
		{
			curves[curves.size() - 1].p2.x_1 = x;
			curves[curves.size() - 1].p2.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p3.x_1 = x;
			curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == CURVE_STATE5)
	{
		if (left_button_up == 1)
		{
			curves[curves.size() - 1].p3.x_1 = x;
			curves[curves.size() - 1].p3.y_1 = CurrentHeight - y;
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
		}
	}
	else if (system_state == CURVE_STATE7)
	{
		if (left_button_up == 1)
		{
			curves[curves.size() - 1].p4.x_1 = x;
			curves[curves.size() - 1].p4.y_1 = CurrentHeight - y;
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
	case POLYGON_STATE_CUT3:
		if (key == 'c')
		{
			polygonCut();
			resizePolygon = 0;
			edit_polygon_point = -1;
			system_state = POLYGON_STATE5;
		}
		break;
	case POLYGON_STATE5:
		if (key == 'c')
		{
			resizePolygon = 0;
			edit_polygon_point = -1;
			system_state = POLYGON_STATE_CUT1;
		}
		else if (key == 'a')
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
	case CURVE_STATE9:
		if (key == 'a')
		{
				curves[curves.size() - 1].p1.x_1 -= 1;
				curves[curves.size() - 1].p2.x_1 -= 1;
				curves[curves.size() - 1].p3.x_1 -= 1;
				curves[curves.size() - 1].p4.x_1 -= 1;
		}
		else if (key == 'd')
		{
			curves[curves.size() - 1].p1.x_1 += 1;
			curves[curves.size() - 1].p2.x_1 += 1;
			curves[curves.size() - 1].p3.x_1 += 1;
			curves[curves.size() - 1].p4.x_1 += 1;
		}
		else if (key == 'w')
		{
			curves[curves.size() - 1].p1.y_1 += 1;
			curves[curves.size() - 1].p2.y_1 += 1;
			curves[curves.size() - 1].p3.y_1 += 1;
			curves[curves.size() - 1].p4.y_1 += 1;
		}
		else if (key == 's')
		{
			curves[curves.size() - 1].p1.y_1 -= 1;
			curves[curves.size() - 1].p2.y_1 -= 1;
			curves[curves.size() - 1].p3.y_1 -= 1;
			curves[curves.size() - 1].p4.y_1 -= 1;
		}
		else if (key == 'q')
		{
			Curve temp = curves[curves.size() - 1];
			int x0 = curves[curves.size() - 1].p1.x_1;
			int y0 = curves[curves.size() - 1].p1.y_1;
			curves[curves.size() - 1].p1.x_1 = (temp.p1.x_1 - x0) * 0 - (temp.p1.y_1 - y0) * 1 + x0;
			curves[curves.size() - 1].p1.y_1 = (temp.p1.x_1 - x0) * 1 - (temp.p1.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p2.x_1 = (temp.p2.x_1 - x0) * 0 - (temp.p2.y_1 - y0) * 1 + x0;
			curves[curves.size() - 1].p2.y_1 = (temp.p2.x_1 - x0) * 1 - (temp.p2.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p3.x_1 = (temp.p3.x_1 - x0) * 0 - (temp.p3.y_1 - y0) * 1 + x0;
			curves[curves.size() - 1].p3.y_1 = (temp.p3.x_1 - x0) * 1 - (temp.p3.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p4.x_1 = (temp.p4.x_1 - x0) * 0 - (temp.p4.y_1 - y0) * 1 + x0;
			curves[curves.size() - 1].p4.y_1 = (temp.p4.x_1 - x0) * 1 - (temp.p4.y_1 - y0) * 0 + y0;
		}
		else if (key == 'e')
		{
			Curve temp = curves[curves.size() - 1];
			int x0 = curves[curves.size() - 1].p1.x_1;
			int y0 = curves[curves.size() - 1].p1.y_1;
			curves[curves.size() - 1].p1.x_1 = (temp.p1.x_1 - x0) * 0 - (temp.p1.y_1 - y0) * (-1) + x0;
			curves[curves.size() - 1].p1.y_1 = (temp.p1.x_1 - x0) * (-1) - (temp.p1.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p2.x_1 = (temp.p2.x_1 - x0) * 0 - (temp.p2.y_1 - y0) * (-1) + x0;
			curves[curves.size() - 1].p2.y_1 = (temp.p2.x_1 - x0) * (-1) - (temp.p2.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p3.x_1 = (temp.p3.x_1 - x0) * 0 - (temp.p3.y_1 - y0) * (-1) + x0;
			curves[curves.size() - 1].p3.y_1 = (temp.p3.x_1 - x0) * (-1) - (temp.p3.y_1 - y0) * 0 + y0;
			curves[curves.size() - 1].p4.x_1 = (temp.p4.x_1 - x0) * 0 - (temp.p4.y_1 - y0) * (-1) + x0;
			curves[curves.size() - 1].p4.y_1 = (temp.p4.x_1 - x0) * (-1) - (temp.p4.y_1 - y0) * 0 + y0;
		}
		else if (key == 'z')
		{
			if (resizeCurve > 0)
			{
					curves[curves.size() - 1].p1.x_1 = curves[curves.size() - 1].p1.x_1 / 2;
					curves[curves.size() - 1].p2.x_1 = curves[curves.size() - 1].p2.x_1 / 2; 
					curves[curves.size() - 1].p3.x_1 = curves[curves.size() - 1].p3.x_1 / 2; 
					curves[curves.size() - 1].p4.x_1 = curves[curves.size() - 1].p4.x_1 / 2; 
					resizeCurve--;
			}
		}
		else if (key == 'x')
		{
			curves[curves.size() - 1].p1.x_1 = curves[curves.size() - 1].p1.x_1 * 2;
			curves[curves.size() - 1].p2.x_1 = curves[curves.size() - 1].p2.x_1 * 2;
			curves[curves.size() - 1].p3.x_1 = curves[curves.size() - 1].p3.x_1 * 2;
			curves[curves.size() - 1].p4.x_1 = curves[curves.size() - 1].p4.x_1 * 2;
			resizeCurve++;
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
	case CURVE:
		system_state = CURVE_STATE1;
		break;
	case CUBE:
		system_state = CUBE_STATE1;
		break;
	default: break;
	}
	glutPostRedisplay();
	changeSize(CurrentWidth, CurrentHeight);
}

void createPopupMenus() {

	int mainMenu = glutCreateMenu(mainMenuProc);

	glutAddMenuEntry("Draw lines", LINE);
	glutAddMenuEntry("Draw circles", CIRCLE);
	glutAddMenuEntry("Draw ellipses", ELLIPSE);
	glutAddMenuEntry("Draw polygons", POLYGON);
	glutAddMenuEntry("Draw filledAreas", FILLEDAREA);
	glutAddMenuEntry("Draw curves", CURVE);
	glutAddMenuEntry("Show Cube", CUBE);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitialWidth - 150, InitialHeight -150);
	glutCreateWindow("Computer Graphics Lab");

	InitEnvironment();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(myMotion);
	glutPassiveMotionFunc(myPassiveMotion);
	glutKeyboardFunc(processNormalKeys);
	glutIdleFunc(renderScene);

	// init Menus
	createPopupMenus();

	// enter GLUT event processing loop
	glutMainLoop();

	return 0;
}