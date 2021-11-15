#include "objectPosition.h"
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdio.h>
//------------------------------------------------------------
//                      ObjectPosition
//------------------------------------------------------------
ObjectPosition::ObjectPosition(void){
    degree=-1;radian=-1;distance=-1;
    status_degree=-1;status_distance=-1;
}
int ObjectPosition::GetDegree(int *pvalue){
    if(status_degree==-1)return 0;
    (*pvalue) = (int)(radian/3.14f*180.0f);
    return 1;
}
int ObjectPosition::GetDistance(int *pvalue){
    if(status_distance==-1)return 0;
    (*pvalue) = (int)distance;
    return 1;
}
void ObjectPosition::Reset(void){status_degree=-1;status_distance=-1;}
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
        if(distance<100)
            return;
        double val=(double)_x/distance;
        if(abs(val)>1){
            return;
        }
        radian = (float)asin(val);
    }
    else{
        _x=_x*cos(1.57-camPos.radian);
        distance=sqrt(_x*_x+_y*_y);
        if(_y==0&&_x==0)
                radian=0;
        else
                radian = atan2(_y,_x);
    }
    //hosei
    int _,__;
    _=distance*sin(radian);
    __=distance*cos(radian);
    __-=camPos.depth;
    distance=std::sqrt(_*_+__*__);
    radian=atan2(_,__);
    status_degree=0;status_distance=0;
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
    //if(_dis>5000)
    //    return;
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
    status_degree=0;status_distance=0;
}

//set about angle
void HolePosition::SetHolePos(int _x,int _y){
    if(abs(_x)>100){
        if(_x>0)
            radian=0.349f;
        else
            radian=-0.349f;
    }
    else{
        radian=0;
    }
    status_degree=0;
}
HolePosition::~HolePosition(){}


