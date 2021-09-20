//#include <GL/gl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "glad.c"

int w=400;
int h=400;
bool first=true;
bool changed=true;
int depth=16;
double sscale=1;
std::vector<std::vector<double>> color;
std::vector<double> trans={0,0};

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
        case GLFW_KEY_KP_ADD:
            sscale+=.1*sscale;
            depth+=2;
            changed=true;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            sscale-=.1*sscale;
            depth-=2;
            changed=true;
            break;
        case GLFW_KEY_H:
            trans={0.488393403505, -0.304677829451};
            changed = 1;
            break;
    }
}

void dynamicResizeWindow(GLFWwindow* window,int iw,int ih) {
	w=iw;
	h=ih;
	first=true;
	changed=true;
	glViewport(0,0,w,h);
}

int mandel(const std::vector<double>& pos,const int& iter_num){
    double x,y,nx,ny;
    x=pos[0];
    y=pos[1];
    for(int i=0;i<iter_num;++i){
        nx=x*x-y*y+pos[0];
        ny=2*x*y+pos[1];
        if(nx*nx+ny*ny>8)
            return i;
        x=nx;
        y=ny;
    }
    return iter_num;
}

int main(){
    // initializing glfw
    glfwInit();
#ifdef __APPLE__
    // uncomment this statement to fix compilation on OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
    // glfw creating window
    GLFWwindow *window = glfwCreateWindow(w,h,"mandelbrot",NULL,NULL);
    if(!window)
        error_occur("failed to create glfw window");
    glfwMakeContextCurrent(window);
    // glad: load all OpenGL function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        error_occur("failed to initialize glad");
    // render loop
    
    color.resize(w*h);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window,dynamicResizeWindow);
    while(!glfwWindowShouldClose(window))
    {
        if(first){
            glClearColor(0.05,0.1,0.1,1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            first=false;
            glfwSwapBuffers(window);
        }
        if(changed){
            color.resize(w*h);
            double ratio2=1.0*h/w;
#pragma omp parallel for
            for(int i=0;i<w*h;++i){
                int x=i%w;
                int y=i/w;
                std::vector<double> pos={(double)x,(double)y};
                int res=mandel({(pos[0]/w*2-1)/sscale*1.5-trans[0]-0.7,(pos[1]/w*2-1+(1-ratio2))/sscale*1.5-trans[1]},depth);
                if(res<depth)
                    color[i]={1.0*res/depth,1.0*res/depth,1.0*res/depth};
                else
                    color[i]={0.0,0.0,0.0};
            }
            for(int i=0;i<w*h;++i){
                int x=i%w;
                int y=i/w;
                glBegin(GL_POINTS);
                glColor3f(color[i][0],color[i][1],color[i][2]);
                glVertex2f(1.0*x/w*2-1,1.0*y/h*2-1);
                glEnd();
            }
            changed=false;
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}