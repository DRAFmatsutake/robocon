#pragma once
#include "moter.h"
#include "camera.h"
#include "ballFinder.h"

class Robot{
    public:
        Robot(void);
        ~Robot();
        void Init(void);
        void Deinit(void);
        int Run(void);
    private:
        Moter *moter;
        Camera *cam,*cam1,*cam2;
        BallFinder *bf,*bf1,*bf2;
        int Manual(void);   //manual mode function

        //camera
        enum CAMERA_ID{
            CAM_1,CAM_2
        };
        int now_cam;
        int NowCam(void);
        void ChangeCam(int cam_id);

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

