#include "objectPosition.h"
#include <math.h>
#include <stdio.h>
//------------------------------------------------------------
//                      ObjectPosition
//------------------------------------------------------------
ObjectPosition::ObjectPosition(void){
    degree=-1;radian=-1;distance=-1;status=-1;
}
int ObjectPosition::GetDegree(int *pvalue){
    if(status==-1)return 0;
    (*pvalue) = (int)(radian/3.14f*180.0f);
    return 1;
}
int ObjectPosition::GetDistance(int *pvalue){
    if(status==-1)return 0;
    (*pvalue) = (int)distance;
    return 1;
}
void ObjectPosition::Reset(void){status=-1;}
ObjectPosition::~ObjectPosition(){}
//------------------------------------------------------------
//                      BallPosition
//------------------------------------------------------------
BallPosition::BallPosition(void){
}
void BallPosition::SetPramater(CameraPosition camPos){
    this->camPos=camPos;
}
BallPosition::~BallPosition(){}

void BallPosition::Set(int _x,int _y,int _dis){
    if(abs(camPos.radian)<0.785){
        distance=(double)sqrt(_dis*_dis-camPos.height*camPos.height);
        double val=(double)_x/distance;
        if(abs(val)>1)return;
        radian = (float)asin(val);
    }
    else{
        _y=_y*cos(camPos.radian);
        distance=sqrt(_x*_x+_y*_y);
        if(_y==0&&_x==0)
                radian=0;
        else
                radian = atan2(_y,-_x);
    }
    //hosei
    int _,__;
    _=distance*sin(radian);
    __=distance*cos(radian);
    __-=camPos.depth;
    distance=sqrt(_*_+__*__);
    radian=atan2(_,__);
    status = 0;
}

//------------------------------------------------------------
//                      HolePosition
//------------------------------------------------------------
HolePosition::HolePosition(void){
    poleHight=400;
}

void HolePosition::SetPramater(CameraPosition camPos){
    this->camPos=camPos;
    poleHeightDiff=poleHight-camPos.height;
}
void HolePosition::SetFlagPos(int _x,int _y,int _dis){
    distance=(double)sqrt(_dis*_dis-poleHeightDiff*poleHeightDiff);
    double val=(double)_x/distance;
    if(abs(val)>1){
        return;
    }
    radian = (float)asin(val);
    //hosei
    int _,__;
    _=distance*sin(radian);
    __=distance*cos(radian);
    __-=camPos.depth;
    distance=sqrt(_*_+__*__);
    radian=atan2(_,__);
    status = 0;
}
HolePosition::~HolePosition(){}


