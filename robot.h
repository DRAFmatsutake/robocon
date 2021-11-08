#pragma once
#include "moter.h"

class Robot{
    public:
        Robot(void);
        ~Robot(void);
        void Init(void);
        void Deinit(void);
        int Run(void);
    private:
        Moter moter;
        int Manual(void);   //manual mode function

        //state buff
        int state;
        int state_pre;
        int Setup();

        //state kinds
        enum STATE{
            EXIT=-1,
            BALL_SEARCH,BALL_FOCUS,BALL_MOVE,
            POLE_SEARCH,SHOT_PREPARE,SHOT_AFTER
        };
        int MainProc(void);
        int State(void);    //decidion state
};

