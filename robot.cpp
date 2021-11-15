#include <stdio.h>
#include "robot.h"
#include "com.h"
#include <unistd.h>     // sleep()
#include <iostream>
#include <sys/sysinfo.h>
#include <wiringPi.h>

//#define MODE_DEBUG
//#define MODE_MANUAL

#define BUTTON_START

Robot::Robot(void){
    state=0;
    state_pre=-1;
    now_cam=CAM_2;
    count=0;
    pole_cam_skip=0;
    preparation=0;
    preparation_skip=0;
    move_skip=0;
    cam1_ball_deg_range=20;
    cam1_ball_dist_range=9;
    cam2_ball_deg_range=7;
    cam2_pole_deg_range=5;
    change_cam_pass=0;
    search_way=0;
    //debug=0;  //use debug
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
    hp=new HolePosition();
    cam1_imgProc=new ImgProcess(cam1,bp,hp,5);
    cam2_imgProc=new ImgProcess(cam2,bp,hp,13);
    cam1_imgProc->SetCameraPosition(230,-40,77);
    cam2_imgProc->SetCameraPosition(300,3,26);

    //bf1=new BallFinder(cam1,5,230,0,70);
    //bf2=new BallFinder(cam2,13,250,3,11);
    ChangeCam(now_cam);
    moter->Update();
    moter->PowerSync(0,0);
    moter->Wheel(0,20);
    usleep(300000);
    pinMode(4,INPUT);
    printf("robot ready please press button\n");
    #ifdef BUTTON_START
        while(digitalRead(4)!=1);
        sleep(1);
    #endif
    printf("program start!!\n");
}
void Robot::Deinit(void){
    usleep(300000);
    moter->Update();
    moter->TimerStop();
    moter->WheelStop();
    usleep(300000);
    moter->Update();
    printf("\nrobot deinit\n");
    delete(moter);
    delete(cam1_imgProc);
    delete(cam2_imgProc);
    delete(bp);
    delete(hp);
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
    #ifndef MODE_MANUAL
    #ifndef MODE_DEBUG
        imgProc->Update();
    #endif
    #endif
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
    /*
    if(NowCam()==CAM_2 && change_cam_pass==0){
        ChangeCam(CAM_1);
        printf("change -> 1\n");
        return 0;
    }
    
    if(state_holder!=PREPARATION && change_cam_pass!=1){
        state_holder=state;
        state=PREPARATION;
        return 0;
    }
    else{
        state_holder=state;
    }
    //ball is found 
    if(bp->GetDegree(&ball_deg)==1 && bp->GetDistance(&ball_dist)==1 && change_cam_pass==0){
        float dis_rad=(float)ball_deg/180.0*3.14f;
        int dis_x=(float)ball_dist*sin(dis_rad);
        int dis_y=(float)ball_dist*cos(dis_rad) + 135;
        //ball is in range
        if(abs(dis_x) <= 75 && abs(dis_y) <= 30){
            state=POLE_SEARCH;
            printf("BALL_SEARCH -> POLE_SEARCH ,dist : %d ,deg : %d ,x_dist : %d ,y_dist : %d\n", ball_dist, ball_deg, dis_x, dis_y);
        }
        else if(abs(dis_x) <= 60){
            state=BALL_MOVE;
            printf("BALL_SEARCH -> BALL_MOVE ,dist : %d ,deg : %d ,x_dist : %d ,y_dist : %d\n", ball_dist, ball_deg, dis_x, dis_y);
        }
        //ball is not in range
        else{
            state=BALL_FOCUS;
            printf("BALL_SEARCH -> BALL_FOCUS ,dist : %d ,deg : %d ,x_dist : %d ,y_dist : %d\n", ball_dist, ball_deg, dis_x, dis_y);
        }
    }*/
    ///*
    int a=-1,b=-1;
    if(bp->GetDistance(&a)!=0&&bp->GetDegree(&b)!=0)
        printf("ball  dis:%d deg:%d  ",a,b);
    if(hp->GetDistance(&a)!=0&&hp->GetDegree(&b)!=0)
        printf("hole  dis:%d deg:%d  ",a,b);
    printf("\n");
    cam->Show("cam");
    //*/
    return 0;
}


int Robot::Manual(void){
    char c;
    c=Func::KeyState();
    if(c=='w'){
        moter->Wheel(70,70);
    }
    else if(c=='s'){
        moter->Wheel(-50,-50);
    }
    else if(c=='a'){
        moter->Wheel(-50,50);
    }
    else if(c=='d'){
        moter->Wheel(50,-50);
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
            hp->SetPramater(imgProc->GetCameraPosition());
        break;
    case CAM_2:
            now_cam=cam_id;
            cam=cam2;
            //bf=bf2;
            imgProc=cam2_imgProc;
            bp->SetPramater(imgProc->GetCameraPosition());
            hp->SetPramater(imgProc->GetCameraPosition());
        break;
    default:
        break;
    }
}
