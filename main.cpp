#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad.c"

int w=400;
int h=400;
float move_x=0.0;
float move_z=0.0;

void error_occur(const char* info){
    std::cout<<info<<std::endl;
    glfwTerminate();
    std::exit(-1);
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
    switch (key){
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_A:
            move_x+=0.1;
            break;
        case GLFW_KEY_D:
            move_x-=0.1;
            break;
        case GLFW_KEY_W:
            move_z+=0.1;
            break;
        case GLFW_KEY_S:
            move_z-=0.1;
            break;
    }
}

void resize_window(GLFWwindow* window,int width,int height){
    w=width;
	h=height;
	glViewport(0,0,w,h);
    return;
}

void init_program(){
    glViewport(0,0,w,h);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
 
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
 
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void init_window(GLFWwindow* window){
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window,key_callback);
    glfwSetFramebufferSizeCallback(window,resize_window);
}

void makeFrustum(double fovY,double aspectRatio,double front,double back){
    const double PI=3.14159265358979323846264338327950288;
    double tangent=std::tan(fovY*PI/360.0);
    double height=front*tangent;
    double width=height*aspectRatio;
    glFrustum(-width,width,-height,height,front,back);
}

void camera(){
    glLoadIdentity();
    makeFrustum(60.0,(double)w/(double)h,1.0,1024.0);
    glTranslatef(move_x,-1.7,move_z);
}

void Draw3d(void)
{
	camera();
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.0f,1.0f,1.0f);
    glVertex3f(1.0,0.0,-2.0);
    glVertex3f(-1.0,0.0,-2.0);
    glVertex3f(-1.0,2.0,-2.0);
    glVertex3f(1.0,2.0,-2.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex3f(1.0,0.0,-2.0);
    glVertex3f(1.0,2.0,-2.0);
    glVertex3f(1.0,2.0,-1.0);
    glVertex3f(1.0,0.0,-1.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(1.0,0.0,-2.0);
    glVertex3f(1.0,0.0,-1.0);
    glVertex3f(-1.0,0.0,-1.0);
    glVertex3f(-1.0,0.0,-2.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f,1.0f,1.0f);
    glVertex3f(1.0,0.0,-12.0);
    glVertex3f(-1.0,0.0,-12.0);
    glVertex3f(-1.0,2.0,-12.0);
    glVertex3f(1.0,2.0,-12.0);
    glEnd();

    glPopMatrix();
}

void render(GLFWwindow* window){
    glClearColor(1.0f,0.8f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    Draw3d();
    return;
}

int main(){
    glfwInit();
    GLFWwindow *window=glfwCreateWindow(w,h,"3d demo",NULL,NULL);
    if(!window)
        error_occur("failed to create glfw window");
    init_window(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        error_occur("failed to initialize glad");
    init_program();

    resize_window(window,w,h);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render(window);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}