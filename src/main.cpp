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

#include "camera.hpp"
#include "block.hpp"
#include "noise.hpp"

Camera camera;
Perlin noise_gen(0.4,4);

int w = 1400;
int h = 800;

const int WORLD_SIZE = 80;
Block world[WORLD_SIZE][WORLD_SIZE];
void world_init() {
    for (int i = 0; i < WORLD_SIZE; ++i)
        for (int j = 0; j < WORLD_SIZE; ++j) {
            world[i][j].set_pos(
                i - WORLD_SIZE / 2.0,
                int(noise_gen.perlin_noise(i * 0.15, j * 0.15) * 10) - 10.0,
                j - WORLD_SIZE / 2.0
            );
        }
    camera.set_position(
        0.0,
        world[WORLD_SIZE / 2][WORLD_SIZE / 2].get_pos().y + 1.7,
        0.0
    );
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

    static float x = 0.0;
    static float z = 0.0;
    auto fly_0 = Block(x - WORLD_SIZE / 2.0, 10, z - WORLD_SIZE / 2.0);
    fly_0.setblock();

    auto fly_1 = Block(x - WORLD_SIZE / 2.0, 10, WORLD_SIZE / 2.0 - z);
    fly_1.setblock();

    auto fly_2 = Block(WORLD_SIZE / 2.0 - x, 10, z - WORLD_SIZE / 2.0);
    fly_2.setblock();

    auto fly_3 = Block(WORLD_SIZE / 2.0 - x, 10, WORLD_SIZE / 2.0 - z);
    fly_3.setblock();

    if (x >= 0)
        x -= 0.1;
    if (z >= 0)
        z -= 0.1;
    if (x < 0)
        x = WORLD_SIZE;
    if (z < 0)
        z = WORLD_SIZE;

    for (int i = 0; i < WORLD_SIZE; ++i)
        for (int j = 0; j < WORLD_SIZE; ++j) {
            world[i][j].setblock();
            auto y = world[i][j].get_pos().y - 1;
            auto b = Block(i - WORLD_SIZE / 2.0, y, j - WORLD_SIZE / 2.0);
            b.setblock();
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