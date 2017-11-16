# 《计算机图形学》系统设计10月进展报告

151220013 陈彧

## 1 系统框架设计

首先大致地描述一下此图形学系统的框架设计。

### 1.1 已完成部分

使用了OpenGL开发环境，安装了GLUT工具包，在程序中通过包含头文件：`#include <GL/glut.h>`，可以调用很多以`gl`、`glu`或`glut`开头的函数，实现相关的绘图等功能。

对于第一次接触OpenGL以及CLUT工具包的自己来说，寻找相关的材料进行适当地学习是很有必要的。我通过Google寻找到了相关学习材料，其中感觉这个[GLUT Tutorial](http://www.lighthouse3d.com/tutorials/glut-tutorial/)的讲解让我收获不小。

通过相关的学习之后，基本确定了如下的一个大致的系统框架代码：

```c++
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
```

#### 1.1.1 初始化函数

其中前5个以`glut`开头的函数起到的作用是初始化GLUT，以及创建窗口。

函数`InitEnvironment()`是对此图形学系统进行相关的环境初始化：

```c++ 
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
```

其中函数`glViewport()`是进行视口的设定，函数`gluOrtho2D()`是进行平面投影坐标系的设定。

#### 1.1.2 回调函数

我们回到`main()`函数中，接下来是登记回调函数，我们挨个地简要阐述这些函数的作用：

函数`glutDisplayFunc(renderScene)`起到的作用有点像MFC中的函数`ondraw()`。我们登记回调函数`renderScene(void)`，其实现如下：

``` c++
void renderScene(void) {

	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < lines.size(); i++)
	{
		drawLines(lines[i].x_1, lines[i].y_1, lines[i].x_2, lines[i].y_2);
	}
	
	for (int i = 0; i < polygons.size(); i++)
	{
		for (int j = 0; j < polygons[i].size();j++)
			drawLines(polygons[i][j].x_1, polygons[i][j].y_1, polygons[i][j].x_2, polygons[i][j].y_2);
	}
	
	glFlush();
}
```

由此我们可以知道，回调函数`renderScene(void)`的作用就是在需要绘制的时候将此图形学系统中的各个图形元素一一输出到屏幕上。

其次是函数`glutReshapeFunc(changeSize)`，其回调函数`void changeSize(int w, int h) `是当窗口大小发生改变时，对应该在窗口中显示的图形元素的大小、比例等显现效果起到一个维护作用，即保持各个图形元素的大小、比例等显现效果在窗口大小改变前后不发生改变。其具体实现在此不赘。

接下来是函数`glutMouseFunc(mouseButton)`，它的回调函数`void mouseButton(int button, int state, int x, int y) `的作用就是可以响应此图形学系统运行过程鼠标操作导致发生的事件。并且我们可以发现传进函数`void mouseButton(int button, int state, int x, int y) `的参数有四个，`int button`是指明鼠标左键、中键或右键，`int state`是指明鼠标是按下或抬起的状态，而`int x`和`int y`是指明鼠标指针在窗口中的位置坐标。在此回调函数中，我们引入了自己创建的一个指明系统状态的变量`system_state`对此图形学系统的运行状态进行记录，同时也比较方便地实现了此回调函数对鼠标相关操作的相应。此回调函数具体实现在此不赘。

然后是函数`glutMotionFunc(myMotion)`，这个函数被触发的条件是，当鼠标的某个键被按下并且鼠标指针在移动，当满足这两个条件时，回调函数`void myMotion(int x, int y)`就会被调用。其具体实现在此不赘。

登记回调函数的最后是函数`glutPassiveMotionFunc(myPassiveMotion)`，这个函数和上一个比较类似，只不过这个函数被触发的条件是当鼠标的某个键松开并且鼠标指针在移动时，回调函数`void myPassiveMotion(int x, int y)`就会被调用。其具体实现在此不赘。

#### 1.1.3 菜单创建函数

函数`createPopupMenus()`主要的作用就是实现弹出菜单的作用，实现代码如下：

``` c++
void createPopupMenus() {

	int mainMenu = glutCreateMenu(mainMenuProc);

	glutAddMenuEntry("Draw lines", LINE);
	glutAddMenuEntry("Draw circles", CIRCLE);
	glutAddMenuEntry("Draw polygons", POLYGON);
	
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
```

我们可以看到，菜单中目前包含画直线、画圆和画多边形3个菜单项，并且我们把弹出菜单这样一个动作和鼠标右键联系起来，即我们可以通过鼠标右键来得到该菜单。

#### 1.1.4 主循环函数

函数`glutMainLoop()`是整个图形学系统的主循环 ，正是因为它，我们的系统才得以不断地运行，并且不断地与用户进行交互。

### 1.2 拟采用部分

打算按照目前的系统框架继续进行完善，比如可能会在之后根据功能添加更多的菜单项，又比如可能会在之后添加相应键盘消息的相关回调函数等。

## 2 系统功能及使用说明

其实我也挺纠结先描述系统功能还是先讲解实现算法的，那就姑且采取由表及里的思路，先描述一下此图形学系统目前实现了哪些功能，之后再基于这些功能讲解它们的实现算法。

### 2.1 已完成部分

#### 2.1.1 绘制直线

目前此图形学系统可以绘制直线。

当进入此系统后，鼠标右键点击窗口界面中除标题栏区域的任一个位置，可以看到会弹出一个菜单，菜单如下图：

![menu](C:\Users\asus\Desktop\图形学10月\menu.png)

我们选择其中的一个菜单项*Draw lines*。

此时，我们便进入了绘制直线的模式。

绘制直线的方式是这样的：鼠标左键点击窗口界面中除标题栏区域的任一个位置，并且不要松开，这时我们就选定了直线的一个端点。再强调一下，我们不要松开鼠标左键，然后移动鼠标，直到鼠标指针到达我们心仪的该直线另一个端点的位置，这时我们松开鼠标，一条直线就绘制完毕了。

可惜PDF不能插入动态图，只能贴一张静态图看看静态的效果：

<img src="C:\Users\asus\Desktop\图形学10月\DrawLine.png" style="zoom:75%" />

#### 2.1.2 编辑直线

关于对直线的编辑，我简单借鉴了一下Windows操作系统里的画图软件，即只能对最后一次绘制的直线进行编辑。

编辑的方式，主要是可以改变直线两个端点的位置，操作方式对用户比较友好，只需用鼠标左键点击直线的某个端点，并且在鼠标左键不松开的情况移动鼠标，将该端点移动至我们心仪的其他地方。

并且此编辑功能可以连续多次使用，直至我们打算画下一条直线。

我们接下来看一下对刚才画的一条直线进行端点移动后的结果：

<img src="C:\Users\asus\Desktop\图形学10月\EDITLine.png" style="zoom:75%" />

#### 2.1.3 绘制多边形

和绘制直线一开始一样，鼠标右键弹出菜单，这时候我们选择菜单项*Draw polygons*。

此时，我们便进入了绘制多边形的模式。

绘制多边形的方式是这样的：鼠标左键点击窗口界面中除标题栏区域的任一个位置，并且松开左键，这时我们就选定了多边形的一个端点。接下来我们发现随着鼠标指针的移动，从刚才选定的顶点到鼠标指针之间总是会产生一条直线，画面效果还是挺棒的。然后我们移动鼠标，直到鼠标指针到达多边形下一个端点的位置，这时我们点击鼠标左键，多边形的第1条边就绘制完毕了。我们按照以上的操作方式，绘制第2条边，绘制第3条边，……直到即将绘制最后1条边时，我们将到鼠标移动到最初开始的端点处，鼠标左键点击之。这样，一个多边形就绘制完毕了。

下面我们来看一下绘制效果：

<img src="C:\Users\asus\Desktop\图形学10月\DrawPolygon.png" style="zoom:75%" />

#### 2.1.4 编辑多边形

编辑多边形的方式和编辑直线比较类似，主要是通过改变多边形某些顶点的位置来改变多边形的形状，操作方式对用户比较友好，只需用鼠标左键点击多边形的某个端点，并且在鼠标左键不松开的情况移动鼠标，将该端点移动至我们心仪的其他地方即可。

并且此编辑功能可以连续多次使用，直至我们打算画下一个多边形。

我们接下来看一下对刚才画的一个多边形进行端点移动后的结果：

<img src="C:\Users\asus\Desktop\图形学10月\EditPolygon.png" style="zoom:75%" />

### 2.2 拟采用部分

拟采用部分就是指现在还没实现，以后想要实现的部分。接下来的话会实现画圆、画曲线、填充区域等功能。

## 3 实现算法

最后谈谈实现的算法。

### 3.1 已完成部分

我觉得要说实现算法，那就姑且也按照系统功能的顺序来大致介绍一下。

#### 3.1.1 绘制直线

关于绘制直线，我运用了*Bresenham算法*，该算法在课上讲得比较易懂，并且在课件中也提供了当直线斜率$k$满足$0 < k < 1$时绘制直线的伪代码。

个人感觉主要就是需要按照直线斜率$k$的取值，进行分类讨论，写出各个情况时绘制图形的不同实现代码即可。以下是当直线斜率$k$满足$0 < k < 1$时绘制直线的C++语言实现代码：

``` c++
/* ... */	
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
/* ... */
```

$k$取其他值时较类似，在此不赘。

#### 3.1.2 编辑直线

编辑直线主要是通过使用回调函数`mouseButton(int button, int state, int x, int y)` 来响应鼠标操作产生的消息事件。每次改变直线某个端点的位置，就重新调用绘制直线的算法来实现直线的重绘：

``` C++
if (button == GLUT_LEFT_BUTTON)
{
	if (state == GLUT_DOWN)
	{
		if ((abs(x - lines[lines.size() - 1].x_1) < 10) && (abs(CurrentHeight - y - lines[lines.size() - 1].y_1) < 10))
		{	//start to edit one end point of the line 
			lines[lines.size() - 1].x_1 = lines[lines.size() - 1].x_2;
         	 lines[lines.size() - 1].y_1 = lines[lines.size() - 1].y_2;
			left_button_down = 1;
			system_state = LINE_STATE2;
		}
		else 
		{	//start to edit the other end point of the line 
             /* ... */
         }
	}
}
```

由以上实现代码可以知道，我们是通过判断鼠标左键点击时，鼠标指针位置是否和直线的某个端点靠得较近来确定用户是否要开始编辑该直线。否则，我们开始绘制下一条直线。

#### 3.1.3 绘制多边形

绘制多边形其实是绘制直线的升级版，相当于绘制很多直线，所以主要涉及了状态机设计的一些知识。

```c++
case POLYGON_STATE1:
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{	//start to draw the polygon
			system_state = POLYGON_STATE2;
			/* ... */
			newLine.x_1 = x;
			newLine.y_1 = CurrentHeight - y;
			newLine.x_2 = x;
			newLine.y_2 = CurrentHeight - y;
			/* ... */
		}
	}
	break;
case POLYGON_STATE2:
	/* ... */
	break;
case POLYGON_STATE3:
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if ((abs(x - polygons[polygons.size() - 1][0].x_1) < 5) && (abs(CurrentHeight - y - polygons[polygons.size() - 1][0].y_1) < 5))
			{	//draw the last line of the polygon
				system_state = POLYGON_STATE4;
				/* ... */
			}
			else
			{	//draw the next line of the polygon   
				system_state = POLYGON_STATE2;
				/* ... */
				newLine.x_1 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2;
				newLine.y_1 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2;
				newLine.x_2 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].x_2;
				newLine.y_2 = polygons[polygons.size() - 1][polygons[polygons.size() - 1].size() - 1].y_2;
				/* ... */
			}
		}
	}
	break;
```

由以上实现代码可以知道绘制多边形过程中状态*POLYGON_STATE1*表示开始绘制多边形的第一条边，状态*POLYGON_STATE3*表示开始绘制多边形的下一条边或最后一条边。

#### 3.1.4 编辑多边形

编辑多边形也和编辑直线比较类似，主要要也是通过使用回调函数`mouseButton(int button, int state, int x, int y)` 来响应鼠标操作产生消息事件。每次改变多边形某个顶点的位置，就重新调用绘制直线的算法来实现与该顶点相邻的两条直线的重绘：

```c++
case POLYGON_STATE4:
	/* ... */
	break;
case POLYGON_STATE5: //edit polygon state
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			int i = 0;
			for (; i < polygons[polygons.size() - 1].size(); i++)
			{    //judge whether one of vertices of the polygon needs to edit
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
			{	//draw the next polygon
				/* ... */
			}
		}
      	/* ... */
	}
	break；
```

由以上实现代码可以知道编辑多边形过程中状态*POLYGON_STATE5*主要表示正在编辑多边形的状态。我们是通过判断鼠标左键点击时，鼠标指针位置是否和多边形某个顶点靠得较近来确定用户是否要开始编辑该多边形。否则，我们开始绘制下一个多边形。

### 3.2 拟采用部分

关于之后可能会使用的算法，可以先展望一下。比如画圆可能会用到中点生成算法，画曲线可能会用到*Bezier曲线绘制*、*B样条曲线绘制*的相关知识，填充区域可能会用到相关的一些填充算法等。