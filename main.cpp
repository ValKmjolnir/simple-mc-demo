#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glad.c"
// set window size
const unsigned int SCR_WIDTH = 400;
const unsigned int SCR_HEIGHT = 300;

void error_occur(const char* info){
    std::cout<<info<<std::endl;
    glfwTerminate();
    std::exit(-1);
}

int main(){
    // initializing glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // uncomment this statement to fix compilation on OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
    // glfw creating window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test<GLFW>", NULL, NULL);
    if (!window)
        error_occur("failed to create glfw window");
    glfwMakeContextCurrent(window);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        error_occur("failed to initialize glad");
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // float vec[3]={0,0,0};
        // for(int i=0;i<100;++i){
        //     for(int j=0;j<100;++j){
        //         for(int k=0;k<100;++k){
        //             glClearColor(vec[0],vec[1],vec[2],1.0f);
        //             glClear(GL_COLOR_BUFFER_BIT);
        //             glfwSwapBuffers(window);
        //             glfwPollEvents();
        //             vec[2]+=0.01;
        //         }
        //         vec[1]+=0.01;
        //     }
        //     vec[0]+=0.01;
        // }
        glClearColor(0.0f,1.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    glfwTerminate();
    return 0;
}