#pragma once

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const double PI = 3.14159265358979323846264338327950288;
const double D2R = PI / 180.0;

class Camera {
private:
    glm::vec3 pos;
    double yaw;
    double pitch;

public:
    Camera(double x = 0, double y = 1.7, double z = 0) {
        pos = {-x, -y, -z};
        yaw = 0;
        pitch = 0;
    }

    Camera(const Camera& c) {
        pos = c.pos;
        yaw = c.yaw;
        pitch = c.pitch;
    }

    void set_position(double x, double y, double z) {
        pos = {-x, -y, -z};
    }

    void move(char m, double length);
    void rotate(const float x_offset, const float y_offset);
    void make_frustum(double fov, double aspect_ratio, double front, double back) const;
    void frustum(double fov, double aspect_ratio, double front, double back) const;
};
