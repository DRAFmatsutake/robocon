#pragma once
#include "moter.h"

class Robot{
    public:
        Robot(void);
        ~Robot();
        void Init(void);
        void Deinit(void);
        int Run(void);
    private:
        Moter moter;
        enum STATE{
            EXIT=-1,
            BALL_SEARCH,BALL_FOCUS,BALL_MOVE,
            POLE_SEARCH,SHOT_PREPARE,SHOT_AFTER
        };
        int State(void);    //状態の決定
        int Manual(void);   //マニュアルモード（デバッグ）
        int state;
        int state_pre;
        int Setup();
};

