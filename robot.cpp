#include <stdio.h>
#include "robot.h"
#include "com.h"

Robot::Robot(void){
    state=1;
    state_pre=-1;
}

Robot::~Robot(void){}

void Robot::Init(void){
    if(R_REDU)
        printf("robot init\n");
    moter.Wheel(0,0);
}
void Robot::Deinit(void){
    moter.Wheel(0,0);
    if(R_REDU)
        printf("robot deinit\n");
}

int Robot::Setup(){
    if(state==state_pre)
        return 0;
    return 1;
}

int Robot::Run(void){
    int r_value = 0;
    moter.Update();

    if(R_MANUAL){
        r_value = Manual();
    }
    else{
        r_value = MainProc();
        state_pre = state;
    }
   return r_value;
}


int Robot::Manual(void){
    char c;
    c=Func::KeyState();
    if(c=='w'){
        moter.Wheel(90,90);
    }
    else if(c=='s'){
        moter.Wheel(-90,-90);
    }
    else if(c=='a'){
        moter.Wheel(-90,90);
    }
    else if(c=='d'){
        moter.Wheel(90,-90);
    }
    else if(c=='r'){
        moter.SetArm(90);
    }
    else if(c=='e'){
        moter.ArmShot();
    }
    else if(c=='f'){
        moter.WheelStop();
    }
    else if(c=='x'){
        moter.TimerWheel(0,-90,1200);
    }
    else if(c=='z'){
        moter.TimerWheel(-90,0,1200);
    }
    else if(c=='q'){
        return 1;
    }
    return 0;
}
