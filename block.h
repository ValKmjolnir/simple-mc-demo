#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Block{
private:
    glm::vec3 pos;
public:
    Block(double x=0,double y=0,double z=0){
        pos={x,y,z};
    }
    Block(const Block& b){
        pos=b.pos;
    }
    void setblock(){
        double x=pos.x;
        double y=pos.y;
        double z=pos.z;

        glPushMatrix();
        glBegin(GL_QUADS);
        // U
        glColor3f(0.0,0.5,0.1);
        glVertex3f(x,y,z);
        glVertex3f(x+1,y,z);
        glVertex3f(x+1,y,z+1);
        glVertex3f(x,y,z+1);
        glEnd();
        // D
        glBegin(GL_QUADS);
        glColor3f(0.5,0.5,0.1);
        glVertex3f(x,y-1,z);
        glVertex3f(x+1,y-1,z);
        glVertex3f(x+1,y-1,z+1);
        glVertex3f(x,y-1,z+1);
        glEnd();
        // B
        glBegin(GL_QUADS);
        glColor3f(0.5,0.5,0.1);
        glVertex3f(x,y-1,z);
        glVertex3f(x+1,y-1,z);
        glVertex3f(x+1,y,z);
        glVertex3f(x,y,z);
        glEnd();
        // F
        glBegin(GL_QUADS);
        glColor3f(0.5,0.5,0.2);
        glVertex3f(x,y-1,z+1);
        glVertex3f(x+1,y-1,z+1);
        glVertex3f(x+1,y,z+1);
        glVertex3f(x,y,z+1);
        glEnd();
        // L
        glBegin(GL_QUADS);
        glColor3f(0.5,0.3,0.1);
        glVertex3f(x,y-1,z);
        glVertex3f(x,y-1,z+1);
        glVertex3f(x,y,z+1);
        glVertex3f(x,y,z);
        glEnd();
        // R
        glBegin(GL_QUADS);
        glColor3f(0.5,0.3,0.1);
        glVertex3f(x+1,y-1,z);
        glVertex3f(x+1,y-1,z+1);
        glVertex3f(x+1,y,z+1);
        glVertex3f(x+1,y,z);
        glEnd();
        glPopMatrix();
    }
};

#endif