#include <stdio.h>
#include "robot.h"
#include "com.h"

#define MODE_MANUAL

Robot::Robot(void){
    state=1;
    state_pre=-1;
}

Robot::~Robot(){}

void Robot::Init(void){
    printf("robot init\n");
    moter=new Moter();
    cam1=new Camera();
    cam2=new Camera();
    cam1->Open(0);
    cam2->Open(2);
    moter->Wheel(0,0);
    printf("robot ready\n");
}
void Robot::Deinit(void){
    moter->Wheel(0,0);
    printf("\nrobot deinit\n");
    delete(moter);
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
    cam1->Update();
    cam2->Update();

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
        cam1->Capture();
    }
    else if(c=='v'){
        cam2->Capture();
    }
    else if(c=='q'){
        return 1;
    }
    return 0;
}
