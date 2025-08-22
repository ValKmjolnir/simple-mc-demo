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

    void move(char m, double length) {
        const auto sin_yaw = sin(yaw * D2R);
        const auto cos_yaw = cos(yaw * D2R);
        switch (m) {
            case 'w': pos -= glm::vec3(length * sin_yaw, 0.0, -length * cos_yaw); break;
            case 's': pos += glm::vec3(length * sin_yaw, 0.0, -length * cos_yaw); break;
            case 'a': pos -= glm::vec3(-length * cos_yaw, 0.0, -length * sin_yaw); break;
            case 'd': pos += glm::vec3(-length * cos_yaw, 0.0, -length * sin_yaw); break;
            case ' ': pos -= glm::vec3(0.0, length, 0.0); break;
            case '~': pos += glm::vec3(0.0, length, 0.0); break;
        }
    }

    void rotate (const float x_offset, const float y_offset) {
        yaw += x_offset * 0.08;
        pitch += y_offset * 0.08;
        if (yaw > 360)
            yaw -= 360;
        else if (yaw < 0)
            yaw += 360;
        if (pitch > 90)
            pitch = 90;
        else if (pitch < -90)
            pitch = -90;
    }

    void make_frustum(double fov, double aspect_ratio, double front, double back) {
        const double tangent = std::tan(fov * PI / 360.0);
        const double height = front * tangent;
        const double width = height * aspect_ratio;
        glFrustum(-width, width, -height, height, front, back);
    }

    void frustum(double fov, double aspect_ratio, double front, double back) {
        glLoadIdentity();
        make_frustum(fov, aspect_ratio, front, back);
        glRotatef(yaw, 0, 1, 0);
        glRotatef(pitch, std::cos(yaw*D2R), 0, std::sin(yaw*D2R));
        glTranslatef(pos.x, pos.y, pos.z);
        return;
    }
};
