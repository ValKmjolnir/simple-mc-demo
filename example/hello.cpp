#include <iostream>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "glad.c" 
#include <GLFW/glfw3.h>   
 
GLuint WIDTH = 400, HEIGHT = 400;
 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void windowResize_callback(GLFWwindow* window, int width, int height);
 
void DrawCoordinate()
{
	glBegin(GL_LINES);
 
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.8, 0, 0);
	glVertex3f(0.8, 0, 0);
 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, -0.8, 0);
	glVertex3f(0, 0.8, 0);
 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, -0.8);
	glVertex3f(0, 0, 0.8);
 
	glEnd();
}
 
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
void Draw3d(void)
{
	int ipvot = 0;
	GLfloat x, y, angle;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
 
	glLoadIdentity();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
 
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);
 
	for (angle = 0; angle <= (2.001f * M_PI); angle += (M_PI / 32.0f))
	{
		x = 0.60f * sin(angle);
		y = 0.60f * cos(angle);
 
		if (ipvot % 2 == 0) glColor3f(0.0f, 1.0f, 1.0f);
		else glColor3f(0.0f, 0.0f, 0.0f);
 
		ipvot++;
 
		glVertex3f(x, y, 0.0f);
	}
 
	glEnd();
 
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.75f);
 
	for (angle = 0.0f; angle <= (2.001f * M_PI); angle += (M_PI / 32.0f))
	{
		x = 0.50f * sin(angle);
		y = 0.50f * cos(angle);
 
		if (ipvot % 2 == 0) glColor3f(0.0f, 1.0f, 0.0f);
		else glColor3f(1.0f, 0.0f, 0.0f);
 
		ipvot++;
 
		glVertex3f(x, y, 0.0f);
 
	}
	glEnd();
 
	DrawCoordinate();
}
 
void render(GLFWwindow* window)
{
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	Draw3d();
}
 
void initWindow(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
 
	//设置按键回调
	glfwSetKeyCallback(window, key_callback);
 
	//设置窗口大小改变的回调，让绘画区域在窗口中间
	glfwSetWindowSizeCallback(window, windowResize_callback);
}
 
void initParam()
{
	//显示规则：窗口左下角坐标为0，0；所以下行代码表示在窗口左下角向右向上的400个像素单位作为画布
	glViewport(0, 0, WIDTH, HEIGHT);//设置显示区域400*400，但是可以拖动改变窗口大小
	glLineWidth(3.0);//设置线条宽度，3个像素
 
	glEnable(GL_BLEND);
 
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//设置点圆滑
 
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);//设置线光滑
 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
 
int main()
{
	glfwInit();
 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
 
	initWindow(window);
 
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
 
	initParam();
 
	// Game loop    
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
 
		render(window);
 
		glfwSwapBuffers(window);
	}
 
	// Terminate GLFW, clearing any resources allocated by GLFW.    
	glfwTerminate();
	return 0;
}
 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_UP)
		xRot -= 5.0f;
 
	else if (key == GLFW_KEY_DOWN)
		xRot += 5.0f;
 
	else if (key == GLFW_KEY_LEFT)
		yRot -= 5.0f;
 
	else if (key == GLFW_KEY_RIGHT)
		yRot += 5.0f;
}
 
void windowResize_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = HEIGHT = width > height ? height : width;
	//左下角是0， 0坐标， x,y说就是相对于左下角的像素距离
	glViewport((width - WIDTH) / 2, (height - HEIGHT) / 2, WIDTH, HEIGHT);
}