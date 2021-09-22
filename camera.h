#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const double PI=3.14159265358979323846264338327950288;

class Camera{
private:
    glm::vec3 pos;
    double yaw;
    double pitch;
public:
    Camera(double x=0,double y=1.7,double z=0){
        pos={-x,-y,-z};
        yaw=pitch=0;
    }
    Camera(const Camera& c){
        pos=c.pos;
        yaw=c.yaw;
        pitch=c.pitch;
    }
    void move(const char m,double length){
        switch(m){
            case 'w':pos-=glm::vec3(length*std::sin(yaw*PI/180),0.0,-length*std::cos(yaw*PI/180));break;
            case 's':pos+=glm::vec3(length*std::sin(yaw*PI/180),0.0,-length*std::cos(yaw*PI/180));break;
            case 'a':pos-=glm::vec3(-length*std::cos(yaw*PI/180),0.0,-length*std::sin(yaw*PI/180));break;
            case 'd':pos+=glm::vec3(-length*std::cos(yaw*PI/180),0.0,-length*std::sin(yaw*PI/180));break;
        }
        std::cout<<-pos.x<<" "<<-pos.y<<" "<<-pos.z<<std::endl;
    }
    void rotate(float x_offset,float y_offset){
        yaw+=x_offset*0.08;
        pitch+=y_offset*0.08;
        if(yaw>360)
            yaw-=360;
        else if(yaw<0)
            yaw+=360;
        if(pitch>90)
            pitch=90;
        else if(pitch<-90)
            pitch=-90;
    }
    void make_frustum(double fov,double aspect_ratio,double front,double back){
        double tangent=std::tan(fov*PI/360.0);
        double height=front*tangent;
        double width=height*aspect_ratio;
        glFrustum(-width,width,-height,height,front,back);
    }
    void frustum(double fov,double aspect_ratio,double front,double back){
        glLoadIdentity();
        make_frustum(fov,aspect_ratio,front,back);
        glRotatef(yaw,0,1,0);
        glRotatef(pitch,std::cos(yaw*PI/180),0,std::sin(yaw*PI/180));
        glTranslatef(pos.x,pos.y,pos.z);
        return;
    }
};

#endif