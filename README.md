# 《计算机图形学》系统设计10月进展报告

151220013 陈彧

## 1 系统框架设计

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

登记回调函数的最后是函数`glutPassiveMotionFunc(myPassiveMotion)`，这个函数和上一个比较类似，只不过这个被触发条件更容易了一些，只需当鼠标指针在移动时，回调函数`void myPassiveMotion(int x, int y)`就会被调用。其具体实现在此不赘。

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

函数`glutMainLoop()`是整个图形学系统的主循环 ，正是因为它，我们的系统才能那个不断得运行，并且不断地与用户进行交互。

### 1.2 拟采用部分

打算按照现目前的系统框架继续进行完善，比如可能会在之后根据功能添加更多的菜单项，又比如可能会在之后添加相应键盘消息的相关回调函数等。

## 2 系统功能

### 2.1 已完成部分

### 2.2 拟采用部分



## 3 实现算法

### 3.1 已完成部分

### 3.2 拟采用部分