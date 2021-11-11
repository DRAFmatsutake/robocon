#include <stdio.h>
#include "robot.h"
#include "com.h"
#include <unistd.h>     // sleep()
#include <iostream>
#include <sys/sysinfo.h>
#include <wiringPi.h>

//#define MODE_DEBUG
//#define MODE_MANUAL

Robot::Robot(void){
    state=0;
    state_pre=-1;
    now_cam=CAM_2;
    cam2_ball_deg_range=10;
    cam2_pole_deg_range=10;
    cam1_ball_dist_range=5;
    debuck=0;
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
    if(cam2->Open(1)==-1)
        cam2->Open(2);
    
    bp=new BallPosition();
    pp=new PolePosition();
    cam1_imgProc=new ImgProcess(cam1,bp,pp,5);
    cam2_imgProc=new ImgProcess(cam2,bp,pp,13);
    cam1_imgProc->SetCameraPosition(230,0,69);
    cam2_imgProc->SetCameraPosition(250,3,29);

    //bf1=new BallFinder(cam1,5,230,0,70);
    //bf2=new BallFinder(cam2,13,250,3,11);
    ChangeCam(now_cam);
    moter->Update();
    moter->Wheel(0,0);
    usleep(300000);
    pinMode(4,INPUT);
    printf("robot ready\n");
}
void Robot::Deinit(void){
    moter->Update();
    moter->TimerStop();
    moter->WheelStop();
    usleep(300000);
    printf("\nrobot deinit\n");
    delete(moter);
    delete(cam1_imgProc);
    delete(cam2_imgProc);
    delete(bp);
    delete(pp);
    //delete(bf1);
    //delete(bf2);
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
    cam->Update();
    moter->Update();

    #ifdef MODE_DEBUG
        Debug();
    #endif

    #ifdef MODE_MANUAL
    #ifndef MODE_DEBUG
        r_value = Manual();
    #endif
    #endif
    #ifndef MODE_MANUAL
    #ifndef MODE_DEBUG
        //bf->Update();
        imgProc->Update();
        r_value = MainProc();
        state_pre = state;
    #endif
    #endif
    /*memory check
    {
        struct sysinfo meminfo;
        ::sysinfo(&meminfo);
        std::cout << meminfo.freeram <<"/"<< meminfo.totalram <<std::endl;
    }
    */
   if(digitalRead(4)==1)
        r_value=1;
   return r_value;
}

int Robot::Debug(void){
    imgProc->Update();
    int a=-1,b=-1;
    bp->GetDegree(&a);
    bp->GetDistance(&b);
    printf("%d %d \n",a,b);
    //cam->Show("cam");
    return 0;
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
            now_cam=cam_id;
            cam=cam1;
            //bf=bf1;
            imgProc=cam1_imgProc;
            bp->SetPramater(imgProc->GetCameraPosition());
            pp->SetPramater(imgProc->GetCameraPosition());
        break;
    case CAM_2:
            now_cam=cam_id;
            cam=cam2;
            //bf=bf2;
            imgProc=cam2_imgProc;
            bp->SetPramater(imgProc->GetCameraPosition());
            pp->SetPramater(imgProc->GetCameraPosition());
        break;
    default:
        break;
    }
}
