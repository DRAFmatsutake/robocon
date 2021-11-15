#pragma once
#include "moter.h"
#include "camera.h"
//#include "ballFinder.h"
#include "imgProcess.h"
#include "objectPosition.h"

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
        //
        ImgProcess *imgProc,*cam1_imgProc,*cam2_imgProc;
        BallPosition *bp;
        HolePosition *hp;
        //BallFinder *bf,*bf1,*bf2;
        int Manual(void);   //manual mode function
        int Debug(void);

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
            EXIT=-1,START_UP,PREPARATION,
            BALL_SEARCH,BALL_FOCUS,BALL_MOVE,BALL_ADJUST,
            POLE_SEARCH,POLE_FOCUS,SHOT_PREPARE,SHOT_AFTER
        };
        int MainProc(void);
        int State(void);    //decidion state

        //use in main_proc
        int count;
        int pole_cam_skip;
        int preparation;
        int state_holder;
        int preparation_skip;
        int set_arm_check;
        int search_way;
        int move_skip;
        int ball_deg;
        int ball_dist;
        int pole_deg;
        int pole_dist;
        int cam1_ball_deg_range;
        int cam1_ball_dist_range;
        int cam2_ball_deg_range;
        int cam2_pole_deg_range;
        int change_cam_pass;
        //int debug; //use debug
};

