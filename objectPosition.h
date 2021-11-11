#pragma once

typedef struct
{
    int height=0;
    int depth=0;
    float radian=0;
} CameraPosition;

//------------------------------------------------------------
//                      ObjectPosition
//------------------------------------------------------------
class ObjectPosition
{
public:
    ObjectPosition(void);
    ~ObjectPosition();
    int GetDegree(int *pvalue);
    int GetDistance(int *pvalue);
    void Reset(void);
protected:
    int degree;
    float radian;
    double distance;
    int status;
};

//------------------------------------------------------------
//                      BallPosition
//------------------------------------------------------------

class BallPosition:public ObjectPosition
{
public:
    BallPosition(void);
    ~BallPosition();
    //camera positon paramater in robot local position
    void SetPramater(CameraPosition camPos);
    //x left:+ right:-
    //y up:- down:+
    void Set(int _x,int _y,int _dis);
private:
    CameraPosition camPos;
};

//------------------------------------------------------------
//                      PolePosition
//------------------------------------------------------------
class PolePosition:public ObjectPosition
{
public:
    PolePosition(void);
    ~PolePosition();
    //camera positon paramater in robot local position
    void SetPramater(CameraPosition camPos);
    int Set(int _x,int _y,int _dis);
private:
    CameraPosition camPos;
};
