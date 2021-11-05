#include <stdio.h>
#include "robot.h"
#include "com.h"

Robot::Robot(void){
    state=1;
    state_pre=-1;
}

Robot::~Robot(){}

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

   moter.Update();

    if(R_MANUAL){
        return Manual();
    }
    else{
        switch (state=State())
        {
            case EXIT:    //終了
                return 1;
                break;
            case BALL_SEARCH:   //ボールを探す
                break;
            case BALL_FOCUS:    //ボールの方向へ向く
                break;
            case BALL_MOVE:     //ボールへ向かう
                break;
            case POLE_SEARCH:   //ポールを探す
                break;
            case SHOT_PREPARE:  //ポールの方向を向く
                break;
            case SHOT_AFTER:    //打ち出し＆後処理
                break;      
            default:
                return -1;
                break;
        }
        state_pre=state;
    }
   return 0;
}

int Robot::State(void){
    return EXIT;
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
