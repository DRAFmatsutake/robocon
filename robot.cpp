#include <stdio.h>
#include "robot.h"
#include "com.h"

#define MODE_MANUAL

Robot::Robot(void){
    state=1;
    state_pre=-1;
    now_cam=CAM_1;
}

Robot::~Robot(){}

void Robot::Init(void){
    printf("robot init\n");
    moter=new Moter();
    cam1=new Camera();
    cam2=new Camera();
    //raspberry
    cam1->Open(0);
    //elecom
    cam2->Open(2);
    bf1=new BallFinder(cam1,5,0,0,0);
    bf2=new BallFinder(cam2,13,0,0,0);
    ChangeCam(now_cam);
    moter->Wheel(0,0);
    printf("robot ready\n");
}
void Robot::Deinit(void){
    moter->Wheel(0,0);
    printf("\nrobot deinit\n");
    delete(moter);
    delete(bf1);
    delete(bf2);
    delete(cam1);
    delete(cam2);
}

int Robot::Setup(){
    if(state==state_pre)
        return 0;
    return 1;
}

int Robot::Run(void){
    int r_value = 0;
    moter->Update();
    cam->Update();
    bf->Update();

    #ifdef MODE_MANUAL
        r_value = Manual();
    #else
        r_value = MainProc();
        state_pre = state;
    #endif
   return r_value;
}


int Robot::Manual(void){
    char c;
    c=Func::KeyState();
    if(c=='w'){
        moter->Wheel(90,90);
    }
    else if(c=='s'){
        moter->Wheel(-90,-90);
    }
    else if(c=='a'){
        moter->Wheel(-90,90);
    }
    else if(c=='d'){
        moter->Wheel(90,-90);
    }
    else if(c=='r'){
        moter->SetArm(90);
    }
    else if(c=='e'){
        moter->ArmShot();
    }
    else if(c=='f'){
        moter->WheelStop();
    }
    else if(c=='x'){
        moter->TimerWheel(0,-90,1200);
    }
    else if(c=='z'){
        moter->TimerWheel(-90,0,1200);
    }
    else if(c=='c'){
        ChangeCam(NowCam()!=CAM_1?CAM_1:CAM_2);
    }
    else if(c=='v'){
        cam->Capture();
    }
    else if(c=='q'){
        return 1;
    }
    return 0;
}

int Robot::NowCam(void){
    return now_cam;
}


void Robot::ChangeCam(int cam_id){
    switch (cam_id)
    {
    case CAM_1:
            printf("use cam1\n");
            now_cam=cam_id;
            cam=cam1;
            bf=bf1;
        break;
    case CAM_2:
            printf("use cam2\n");
            now_cam=cam_id;
            cam=cam2;
            bf=bf2;
        break;
    default:
        break;
    }
}