#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void setblock_pos(const double x,const double y,const double z){
    glPushMatrix();
    glTranslatef(x,y,z);
    glBegin(GL_QUADS);
    // U
    glColor3f(0.0,0.5,0.1);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,0,1);
    glVertex3f(0,0,1);
    // D
    glColor3f(0.3,0.1,0.05);
    glVertex3f(0,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,-1,0);
    glVertex3f(0,-1,0);
    // B
    glColor3f(0.5,0.3,0.1);
    glVertex3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,-1,0);
    glColor3f(0.5,0.2,0.1);
    glVertex3f(1,-1,0);
    // F
    glColor3f(0.5,0.3,0.1);
    glVertex3f(0,0,1);
    glVertex3f(1,0,1);
    glVertex3f(1,-1,1);
    glColor3f(0.5,0.2,0.1);
    glVertex3f(0,-1,1);
    // L
    glColor3f(0.5,0.3,0.1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glVertex3f(0,-1,1);
    glColor3f(0.5,0.2,0.1);
    glVertex3f(0,-1,0);
    // R
    glColor3f(0.5,0.3,0.1);
    glVertex3f(1,0,0);
    glVertex3f(1,-1,0);
    glVertex3f(1,-1,1);
    glColor3f(0.5,0.2,0.1);
    glVertex3f(1,0,1);

    glEnd();
    glPopMatrix();
}

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
    const glm::vec3& get_pos(){
        return pos;
    }
    void set_pos(double x,double y,double z){
        pos={x,y,z};
    }
    void modern_setblock(){
        float vertices[]={
            0,0,0,//0
            1,0,0,//1
            1,1,0,//2
            0,1,0,//3
            0,0,-1,//4
            1,0,-1,//5
            1,1,-1,//6
            0,1,-1//7
        };
        uint32_t indices[]={
            0,1,3,//f
            3,1,2,
            1,5,2,//r
            2,5,6,
            5,4,6,//b
            6,4,7,
            4,0,7,//l
            7,0,3,
            3,2,7,//u
            7,2,6,
            0,4,1,//d
            1,4,5
        };
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,nullptr);
        glBindVertexArray(0);
    }
    void setblock(){
        setblock_pos(pos.x,pos.y,pos.z);
    }
};

#endif