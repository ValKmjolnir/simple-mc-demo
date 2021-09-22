#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad.c"
#include "camera.h"
#include "block.h"
#include "noise.h"

Camera player;
Perlin noise_gen(0.4,4);

int w=1080;
int h=600;

Block world[65][65];
void world_init(){
    for(int i=0;i<65;++i)
        for(int j=0;j<65;++j)
            world[i][j].set_pos(i-32.0,int(noise_gen.perlin_noise(i/4.5,j/4.5)*5)-5.0,j-32.0);
}

void error_occur(const char* info){
    std::cout<<info<<std::endl;
    glfwTerminate();
    std::exit(-1);
}

void process_input(GLFWwindow *window)
{
    static float delta_time=0;
    static float last_frame=0;

    float current_frame=glfwGetTime();
    delta_time=current_frame-last_frame;
    last_frame=current_frame;
    float camera_speed=6*delta_time;
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
        player.move('w',camera_speed);
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
        player.move('s',camera_speed);
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
        player.move('a',camera_speed);
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
        player.move('d',camera_speed);
}

void mouse_callback(GLFWwindow* window,double xpos,double ypos){
    static bool first=true;
    static double last_x=w/2.0;
    static double last_y=h/2.0;
    if(first){
        last_x=xpos;
        last_y=ypos;
        first=false;
    }
    float x_offset=xpos-last_x;
    float y_offset=ypos-last_y;
    last_x=xpos;
    last_y=ypos;
    player.rotate(x_offset,y_offset);
    if(xpos>=0.75*w || xpos<=0.25*w || ypos>=0.75*h || ypos<=0.25*h){
        glfwSetCursorPos(window,w/2.0,h/2.0);
        first=true;
    }
}

void resize_window(GLFWwindow* window,int width,int height){
    w=width;
	h=height;
	glViewport(0,0,w,h);
    return;
}

void light(){
    const GLfloat lightposition[]={0,-3,6,0};
    static GLfloat ambientlight[]={0.6,0.6,0.6,0.6};
    static float x=1;
    const GLfloat diffuselight[]={0.6,0.6,0.6,0.6};
    const GLfloat specularlight[]={0.2,0.2,0.2,0.2};

    const GLfloat mat_ambient[]={0.1,0.1,0.1,1};
    const GLfloat mat_diffuse[]={0.6,0.6,0.6,0.6};
    const GLfloat mat_specular[]={0.2,0.2,0.2,0.2};
    const GLfloat mat_shininess[]={1.0};

    for(int i=0;i<4;++i)
        ambientlight[i]+=x*0.003;
    if(ambientlight[0]>=1)
        x=-1;
    else if(ambientlight[0]<=0)
        x=1;

    glLightfv(GL_LIGHT0,GL_POSITION,lightposition);

	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientlight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuselight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularlight);

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void init_program(){
    glViewport(0,0,w,h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_MODELVIEW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
 
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
    
    light();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void init_window(GLFWwindow* window){
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,resize_window);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}

void Draw3d(void)
{
	player.frustum(60,(double)w/(double)h,1.0,1024.0);
    light();

    for(int i=0;i<65;++i)
        for(int j=0;j<65;++j){
            world[i][j].setblock();
            for(int k=world[i][j].get_pos().y-1;k>-9;--k){
                Block a(i-32.0,k-0.0,j-32.0);
                a.setblock();
            }
        }
}

void render(GLFWwindow* window){
    glClearColor(0.9f,0.9f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    Draw3d();
    return;
}

auto main()->int{
    world_init();
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
        process_input(window);
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}