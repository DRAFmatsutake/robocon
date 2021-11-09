#include "ballFinder.h"
#include <math.h>


BallFinder::BallFinder(Camera *cp,int parameter,int height_distance,int depth_distance,int camera_degree)
{
    this->cp = cp;
    this->parameter = parameter;
    this->height_distance = height_distance;
    this->depth_distance = depth_distance;
    this->camera_radian = (float)camera_degree*3.14f/180.0f;
}

BallFinder::~BallFinder()
{
}

void BallFinder::Update(void){

    //camera local to robot rocal
    distance=distance*cos(camera_radian);
}

int BallFinder::GetDegree(void){
    return degree;
}
int BallFinder::GetDistance(void){
    return distance;
}