#pragma once
#include "camera.h"

class BallFinder
{
public:
    //parameter
    //height_distance up=+ down=-
    //depth_distance front=+ back=-
    //camera_degree front = 0 up=+ down=-
    BallFinder(Camera *cp,int parameter,int height_distance,int depth_distance,int camera_degree);
    ~BallFinder();
    void Update(void);
    int GetDistance(void);
    int GetDegree(void);
private:
    Camera *cp;
    int parameter;
    int height_distance;
    int depth_distance;
    float camera_radian;

    int distance;
    int degree;
};

