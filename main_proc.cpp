#include "robot.h"

//end procces 1
//continue procces 0
//error -1

int Robot::MainProc(void){
    
        switch (state=State())
        {
            case EXIT:    
                return 1;
                break;
            case BALL_SEARCH:   
                break;
            case BALL_FOCUS:    
                break;
            case BALL_MOVE:     
                break;
            case POLE_SEARCH:   
                break;
            case SHOT_PREPARE:  
                break;
            case SHOT_AFTER:    
                break;      
            default:
                return -1;
                break;
        }
        return 0;
}


int Robot::State(void){
    return EXIT;
}