#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint VAO;
GLuint VBO;
GLuint EBO;

#include "glad.c"
#include "camera.h"
#include "block.h"
#include "noise.h"

Camera camera;
Perlin noise_gen(0.4,4);

int w=1080;
int h=600;

Block world[65][65];
void world_init() {
    for(int i=0;i<65;++i)
        for(int j=0;j<65;++j){
            world[i][j].set_pos(i-32.0,int(noise_gen.perlin_noise(i*0.15,j*0.15)*10)-10.0,j-32.0);
        }
    camera.set_position(0.0,world[32][32].get_pos().y+1.7,0.0);
}

void error_occur(const char* info) {
    std::cout << info << std::endl;
    glfwTerminate();
    std::exit(-1);
}

void process_input(GLFWwindow *window) {
    static float delta_time = 0;
    static float last_frame = 0;

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    float camera_speed = 12 * delta_time;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move('w', camera_speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move('s', camera_speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move('a', camera_speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move('d', camera_speed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.move(' ', camera_speed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.move('~', camera_speed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first = true;
    static double last_x = w / 2.0;
    static double last_y = h / 2.0;
    if (first) {
        last_x = xpos;
        last_y = ypos;
        first = false;
    }
    float x_offset = xpos-last_x;
    float y_offset = ypos-last_y;
    last_x = xpos;
    last_y = ypos;
    camera.rotate(x_offset, y_offset);
}

void resize_window(GLFWwindow* window, int width, int height) {
    w = width;
    h = height;
    glViewport(0, 0, w, h);
    return;
}

void light() {
    const GLfloat lightposition[] = {0, 0, 0, 0};
    static GLfloat ambientlight[] = {0.6, 0.6, 0.6, 1};
    static GLfloat diffuselight[] = {0.8, 0.8, 0.8, 1};
    const GLfloat specularlight[] = {0.8, 0.8, 0.8, 1};

    const GLfloat mat_ambient[]   = {0.6, 0.6, 0.6, 1};
    const GLfloat mat_diffuse[]   = {0.8, 0.8, 0.8, 1};
    const GLfloat mat_specular[]  = {0.8, 0.8, 0.8, 1};
    const GLfloat high_mat[]      = {0.1, 0.1, 0.1, 0.5};
    const GLfloat mat_shininess[] = {16.0};

    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularlight);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, high_mat);
}

void init_program() {
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_MODELVIEW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    light();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void init_window(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Draw3d(void) {
    camera.frustum(60, (double)w/(double)h, 0.1, 512.0);
    light();

    for (int i = 0; i < 65; ++i)
        for (int j = 0; j < 65; ++j) {
            world[i][j].setblock();
            auto y = world[i][j].get_pos().y - 1;
            for (int k = 0; k < 2; ++k) {
                auto b = Block(i - 32.0, y - k, j - 32.0);
                b.setblock();
            }
        }
}

void render(GLFWwindow* window) {
    glClearColor(0.9f, 0.9f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Draw3d();
    return;
}

int main() {
    world_init();
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(w, h, "minecraft demo", nullptr, nullptr);
    if (!window)
        error_occur("failed to create glfw window");
    init_window(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        error_occur("failed to initialize glad");
    init_program();

    resize_window(window, w, h);
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}