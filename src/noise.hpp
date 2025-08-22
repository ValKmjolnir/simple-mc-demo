#pragma once

#include <cmath>

class Perlin {
private:
    float persistance;
    int octave_num;

private:
    double noise(int x,int y) {
        int n=x+y*57;
        n=(n>>13)^n;
        return (1.0-((n*(n*n*15731+789221)+1376312589)&0x7fffffff)/1073741824.0);
    }
    double smoothed_noise(int x,int y){
        double corners=(noise(x-1,y-1)+noise(x+1,y-1)+noise(x-1,y+1)+noise(x+1,y+1))/16;
        double sides=(noise(x-1,y)+noise(x+1,y)+noise(x,y-1)+noise(x,y+1))/8;
        double center=noise(x,y)/4;
        return corners+sides+center;
    }
    double cosine_interpolate(double a,double b,double x) {
        double f=(1-std::cos(x*3.14159265358979323846264338327950288))*0.5;
        return a*(1-f)+b*f;
    }
    double linear_interpolate(double a,double b,double x) {
        return a*(1-x)+b*x;
    }
    double interpolated_noise(float x,float y) {
        float fractional_x=x-int(x);
        float fractional_y=y-int(y);
        double v1=smoothed_noise((int)x,(int)y);
        double v2=smoothed_noise((int)x+1,(int)y);
        double v3=smoothed_noise((int)x,(int)y+1);
        double v4=smoothed_noise((int)x+1,(int)y+1);
        double i1=cosine_interpolate(v1,v2,fractional_x);
        double i2=cosine_interpolate(v3,v4,fractional_x);
        return cosine_interpolate(i1,i2,fractional_y);
    }

public:
    Perlin(float _persistence=0.5,int _octave_num=4){
        persistance=_persistence;
        octave_num=_octave_num;
    }
    double perlin_noise(float x,float y){
        double noise=0;
        for(int i=0;i<octave_num;++i){
            double frequency=std::pow(2,i);
            double amplitude=std::pow(persistance,i);
            noise+=interpolated_noise(x*frequency,y*frequency)*amplitude;
        }
        return noise;
    }
};
