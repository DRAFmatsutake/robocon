#include "robot.h"
#include <math.h>

//end procces 1
//continue procces 0
//error -1

int Robot::MainProc(void){
    ///*
        //if(moter->TimerWheelState()==false && debug==0)
        //    moter->TimerWheel(90,90,3000);
        //    debug=1;

        switch (state=State())
        {
            //-------------------------------------------------
            //End Procces
            //-------------------------------------------------
            case EXIT:
                //if(moter->TimerWheelState()==true)
                //    break;
                return 1;
                break;
        
            //-------------------------------------------------
            //Start Up
            //-------------------------------------------------
            case START_UP:
                if(moter->SetArm(90)!=1){
                    state=START_UP;
                    break;
                }
                else{
                    int arm_deg_check;
                    arm_deg_check=(int)moter->GetArmDegree();
                    if(arm_deg_check <= 55){
                        set_arm_check=moter->SetArm(60);
                        state=START_UP;
                        break;
                    }
                }
                state=BALL_SEARCH;
                state_holder=START_UP;
                //state=POLE_SEARCH;
                break;
            
            //-------------------------------------------------
            //Preparation
            //-------------------------------------------------
            case PREPARATION:
                if(preparation < 7){
                    preparation++;
                    state=PREPARATION;
                    break;
                }
                preparation=0;
                state=state_holder;
                state_holder=PREPARATION;
                break;

            //-------------------------------------------------
            //Ball Search
            //-------------------------------------------------
            case BALL_SEARCH:
                if(NowCam()==CAM_2 && change_cam_pass==0){
                    ChangeCam(CAM_1);
                    printf("change -> 1\n");
                    break;
                }
                
                if(state_holder!=PREPARATION && change_cam_pass!=1){
                    state_holder=state;
                    state=PREPARATION;
                    break;
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
                }
                //ball is not found
                else{
                    if(NowCam()==CAM_1 && change_cam_pass==0){
                        ChangeCam(CAM_2);
                        change_cam_pass=1;
                        printf("change -> 2\n");
                        break;
                    }
                    if(state_holder!=PREPARATION && change_cam_pass!=1){
                        state_holder=state;
                        state=PREPARATION;
                        break;
                    }
                    else{
                        state_holder=state;
                    }
                    //CAM1 found ball
                    if(bp->GetDegree(&ball_deg)==1 && bp->GetDistance(&ball_dist)==1){
                        //ball is in range
                        if((ball_deg > -cam2_ball_deg_range && ball_deg < cam2_ball_deg_range) || (ball_deg > 180 -cam2_ball_deg_range && ball_deg < -180 + cam2_ball_deg_range || ball_dist < cam1_ball_dist_range)){
                            state=BALL_MOVE;
                            printf("BALL_SEARCH -> BALL_MOVE ,CAM_2 ,%d\n", ball_deg);
                            change_cam_pass=0;
                        }
                        //ball is not in range
                        else{
                            state=BALL_FOCUS;
                            printf("BALL_SEARCH -> BALL_FOCUS ,CAM_2 ,%d\n", ball_deg);
                            change_cam_pass=0;
                        }
                    }
                    //CAM1 did'nt found ball
                    else{
                        cam2_ball_deg_range = 6;
                        if(moter->TimerWheelState()==false){
                            if(search_way==0){
                                moter->TimerWheel(-70,70,25); //sukoshi mawaru
                                search_way=1;
                            }
                            else if(search_way==1){
                                moter->TimerWheel(70,-70,45); //sukoshi modoru
                                search_way=2;
                            }
                            else{
                                moter->TimerWheel(-70,-70,70); //sukoshi modoru
                                search_way=0;
                            }
                        }
                        state=BALL_SEARCH;
                        printf("BALL_SEARCH -> BALL_SEARCH ,NOT FOUND\n");
                        change_cam_pass=0;
                    }
                }  
                break;
                
            //-------------------------------------------------
            //Ball Focus
            //-------------------------------------------------
            case BALL_FOCUS:
                //ball is in plus range
                if(ball_deg < 0 && ball_deg >= -180){
                    int move_left_time = (abs(ball_deg)) * 0.2;
                    if(moter->TimerWheelState()==false){
                        moter->TimerWheel(-60,60,move_left_time); //deg wo ** suru
                        printf("Left, %d\n", move_left_time);
                    }
                }
                //ball is in minus range
                else if(ball_deg >= 0 && ball_deg <= 180){
                    int move_right_time = (abs(ball_deg)) * 0.2;
                    if(moter->TimerWheelState()==false){
                        moter->TimerWheel(60,-60,move_right_time); //deg wo ** suru
                        printf("Right, %d\n", move_right_time);
                    }
                }
                cam2_ball_deg_range = 5;
                state=BALL_SEARCH;
                printf("BALL_FOCUS -> BALL_SEARCH\n");
                break;
            
            //-------------------------------------------------
            //Ball Move
            //-------------------------------------------------
            case BALL_MOVE: 
                if(state_holder!=PREPARATION && preparation_skip!=1){
                    state_holder=state;
                    state=PREPARATION;
                    break;
                }
                else{
                    state_holder=state;
                }
                //ball is found 
                if(bp->GetDistance(&ball_dist)==1){
                    printf("MOVE ,find\n");
                    if((ball_dist < -cam1_ball_dist_range || ball_dist > cam1_ball_dist_range) && move_skip!=1){
                        printf("MOVE ,move\n");
                        if((double)(ball_dist + 150) * 9.5 > 6000){
                            int move_front_time = (ball_dist) * 5;
                            if(moter->TimerWheelState()==false) moter->TimerWheel(70,70,move_front_time); //(dist - cam2 no gakaku) bun susumu
                            printf("Front_harf ,%d\n", ball_dist);
                        }
                        else if(ball_dist * 10 < 200){
                            state=POLE_SEARCH;
                            printf("BALL_MOVE -> POLE_SEARCH\n");
                        }
                        else{
                            if(NowCam()==CAM_1){
                                int move_front_time = (ball_dist) * 1.2;
                                if(moter->TimerWheelState()==false) moter->TimerWheel(70,70,move_front_time); //(dist - cam2 no gakaku) bun susumu
                                printf("Front_cam1 ,%d\n", ball_dist);
                            }
                            else{
                                int move_front_time = (ball_dist + 150) * 8;
                                if(moter->TimerWheelState()==false) moter->TimerWheel(70,70,move_front_time); //(dist - cam2 no gakaku) bun susumu
                                printf("Front_large ,%d\n", ball_dist);
                            }
                        }
                    }

                    if(NowCam()==CAM_2){
                        ChangeCam(CAM_1);
                        move_skip=1;
                        printf("change m1 c1\n");
                        break;
                    }
                    move_skip=1;
                    if(state_holder!=PREPARATION && preparation_skip!=1){
                        state_holder=state;
                        preparation_skip=1;
                        state=PREPARATION;
                        break;
                    }
                    else{
                        state_holder=state;
                        preparation_skip=0;
                    }
                    //CAM1 found ball
                    if(bp->GetDegree(&ball_deg)==1 && bp->GetDistance(&ball_dist)==1){
                        move_skip=0;
                        //ball is in correct position
                        if(ball_dist > -(cam1_ball_dist_range) && ball_dist < (cam1_ball_dist_range)){
                            if(NowCam()==CAM_1){
                                ChangeCam(CAM_2);
                                printf("change m3 c2\n");
                                state=POLE_SEARCH;
                                printf("BALL_MOVE -> POLE_SEARCH\n");
                                /*
                                printf("BALL_MOVE -> POLE_SEARCH, %d\n", ball_dist);
                                int shot;
                                if(moter->TimerWheelState()==false) shot=moter->ArmShot();
                                printf("shot : %d\n", shot);
                                if(shot==1) state=SHOT_AFTER;
                                *///*
                                break;
                            }
                            state=POLE_SEARCH;
                            printf("BALL_MOVE -> POLE_SEARCH\n");
                            /*
                            printf("BALL_MOVE -> POLE_SEARCH, %d\n", ball_dist);
                            int shot;
                            if(moter->TimerWheelState()==false) shot=moter->ArmShot();
                            printf("shot : %d\n", shot);
                            if(shot==1) state=SHOT_AFTER;
                            *///*
                        }
                        //ball is not in correct position
                        else{
                            state=BALL_SEARCH;
                            //state=BALL_ADJUST;
                            //printf("BALL_MOVE -> BALL_ADJUST\n");
                        }
                    }
                    //CAM1 did'nt find ball
                    else{
                        move_skip=0;
                        if(moter->TimerWheelState()==false){
                            moter->TimerWheel(70,70,3500); //susumu
                            printf("dead\n");
                        }
                        if(NowCam()==CAM_1){
                            ChangeCam(CAM_2);
                            printf("change m4 c2\n");
                            break;
                        }
                        state=BALL_SEARCH;
                        printf("BALL_MOVE -> BALL_SEARCH\n");
                    }
                }
                //ball is not found
                else{
                    move_skip=0;
                    state=BALL_SEARCH;
                    printf("BALL_MOVE -> BALL_SEARCH\n");
                }
                break;
            
            //-------------------------------------------------
            //Ball Adjust
            //-------------------------------------------------
            case BALL_ADJUST:
                int checker;
                if(Setup()==0){
                    if(state_holder!=PREPARATION){
                        state_holder=state;
                        state=PREPARATION;
                        break;
                    }
                    else{
                        state_holder=state;
                    }
                    checker=bp->GetDegree(&ball_deg);
                    if((ball_deg < -cam1_ball_deg_range && ball_deg > -180 + cam1_ball_deg_range) || (ball_deg > cam1_ball_deg_range && ball_deg < 180 - cam1_ball_deg_range)){
                    if(ball_deg >= 0 && ball_deg <= 180){
                        int move_left_time = (abs(ball_deg)) * 0.2;
                        if(moter->TimerWheelState()==false) moter->TimerWheel(0,-60,move_left_time);
                        printf("Turn Left, %d\n", ball_deg);
                    }
                    else if(ball_deg >= -180 && ball_deg < 0){
                        int move_right_time = (abs(ball_deg)) * 0.2;
                        if(moter->TimerWheelState()==false) moter->TimerWheel(-60,0,move_right_time);
                        printf("Turn Right, %d\n", ball_deg);
                    }
                }
                }
                if(checker==0){
                    state=BALL_SEARCH;
                    break;
                }
                else{
                    if(Setup()==0){
                        if(state_holder!=PREPARATION){
                            state_holder=state;
                            state=PREPARATION;
                            break;
                        }
                        else{
                            state_holder=state;
                        }
                        checker=bp->GetDistance(&ball_deg);
                    }
                    if(checker==0){
                        state=BALL_SEARCH;
                        break;
                    }
                    if(ball_dist < -(cam1_ball_dist_range/10) && ball_dist > (cam1_ball_dist_range/10)){
                        int move_time = 0;
                        move_time=ball_dist*0.3;
                        if(moter->TimerWheelState()==false){
                            moter->TimerWheel(60,60,move_time);    //robot center ni awaseru, front
                            printf("Little Front, %d\n", ball_dist);
                        }
                    }
                    else{
                        state=POLE_SEARCH;
                        printf("BALL_ADJUS -> POLE_SEARCH\n");
                        /*
                        printf("BALL_ADJUS -> POLE_SEARCH\n");
                        int shot;
                        if(moter->TimerWheelState()==false) shot=moter->ArmShot();
                        printf("shot : %d\n", shot);
                        if(shot==1) state=SHOT_AFTER;
                        *///*
                        break;
                    }
                }
                state=BALL_ADJUST;
                printf("BALL_ADJUS -> BALL_ADJUST\n");
                break;
            //*/ 
            //-------------------------------------------------
            //Pole Search
            //-------------------------------------------------
            case POLE_SEARCH:
                if(NowCam()==CAM_1 && pole_cam_skip!=1){
                    ChangeCam(CAM_2);
                    printf("change p c2\n");
                    break;
                }
                if(state_holder!=PREPARATION && pole_cam_skip!=1){
                    state_holder=state;
                    state=PREPARATION;
                    count++;
                    if(count>5){
                        //state_holder=PREPARATION;
                    }
                    printf("POLE_SEARCH -> PREPARATION1\n");
                    break;
                }
                else{
                    state_holder=state;
                    count=0;
                }
                //pole is found
                if(hp->GetDegree(&pole_deg)==1){
                    printf("pole_deg : %d\n", pole_deg);
                    //pole is in range
                    if(pole_deg >= -cam2_pole_deg_range && pole_deg <= cam2_pole_deg_range){
                        printf("pole found\n");
                        if(NowCam()==CAM_2){
                            ChangeCam(CAM_1);
                            pole_cam_skip=1;
                            printf("change p c1\n");
                            break;
                        }
                        if(state_holder!=PREPARATION && pole_cam_skip!=1){
                            state_holder=state;
                            state=PREPARATION;
                            pole_cam_skip=1;
                            count++;
                            if(count>5){
                               //state_holder=PREPARATION;
                            }
                            printf("POLE_SEARCH -> PREPARATION2\n");
                            break;
                        }
                        else{
                            state_holder=state;
                            pole_cam_skip=0;
                            count=0;
                        }
                        if(bp->GetDegree(&ball_deg)==1 && bp->GetDistance(&ball_dist)==1 && change_cam_pass==0){
                            printf("ball found\n");
                            float dis_rad=(float)ball_deg/180.0*3.14f;
                            int dis_x=(float)ball_dist*sin(dis_rad);
                            int dis_y=(float)ball_dist*cos(dis_rad) + 135;
                            //ball is in range
                            if(abs(dis_x) <= 75 && abs(dis_y) <= 30){
                                state=SHOT_PREPARE;
                                printf("POLE_SEARCH -> SHOT_PREPARE\n");
                            }
                            else{
                                state=BALL_SEARCH;
                                printf("POLE_SEARCH -> BALL_SEARCH\n");
                            }
                        }
                        else{
                            state=BALL_SEARCH;
                            printf("POLE_SEARCH -> BALL_SEARCH\n");
                        }
                    }
                    //pole is not in range
                    else{
                        state=POLE_FOCUS;
                        printf("POLE_SEARCH -> POLE_FOCUS\n");
                    }
                }
                //pole is not found
                else{
                    cam2_pole_deg_range = 1;
                    if(moter->TimerWheelState()==false) moter->TimerWheel(70,-70,25); //sukoshi mawaru
                    state=POLE_SEARCH;
                    printf("POLE_SEARCH -> POLE_SEARCH\n");
                }   
                break;
            
            //-------------------------------------------------
            //Pole Focus
            //-------------------------------------------------
            case POLE_FOCUS:
                //pole is in plus range
                if(pole_deg <= 0){
                    int move_left_time = (abs(pole_deg)) * 0.2;
                    if(moter->TimerWheelState()==false) moter->TimerWheel(-70,70,move_left_time); //deg wo ** suru
                    printf("Left, %d\n", move_left_time);
                }
                //pole is in minus range
                else{
                    int move_right_time = (abs(pole_deg)) * 0.2;
                    if(moter->TimerWheelState()==false) moter->TimerWheel(70,-70,move_right_time); //deg wo ** suru
                    printf("Right, %d\n", move_right_time);
                }
                cam2_pole_deg_range = 1;
                state=POLE_SEARCH;
                printf("POLE_FOCUS -> POLE_SEARCH\n");
                break;
            
            //-------------------------------------------------
            //Shot Prepare
            //-------------------------------------------------
            case SHOT_PREPARE:
                int shot;
                if(moter->TimerWheelState()==false){
                    if(moter->ArmShot()!=1){
                        state=SHOT_PREPARE;
                        break;
                    }
                }
                state=SHOT_AFTER;

                /*
                ChangeCam(CAM_2);
                //CAM2 found ball
                if(bp->GetDegree(ball_deg)==1 && bp->GetDistance(ball_dist)==1){
                    //ball is not in correct position
                    if(abs(ball_dist * cos(ball_deg * 3.141592 / 180)) < 10){
                        moter->TimerWheel(90,90,1000);    //zengo nomi de utidashi iti ni awaseru
                    }
                    //pole is found
                    if(hp->GetDistance(pole_dist)==1){
                        moter->SetArm(90);//power is calculated from pole_dist
                    }
                    //pole is not found
                    else{
                        moter->SetArm(45);
                    }
                    moter->ArmShot();
                    ChangeCam(CAM_1);
                    state=SHOT_AFTER;
                }
                //CAM2 did'nt find ball
                else{
                    moter->TimerWheel(-90,-90,1000);    //CAM2 no gakaku plus arufa back
                    ChangeCam(CAM_1);
                    state=BALL_SEARCH;
                }*/

                break;
                
            //-------------------------------------------------
            //Shot After
            //-------------------------------------------------
            case SHOT_AFTER:
                printf("SHOT_AFTER\n");
                if(moter->SetArm(60)!=1){
                    state=SHOT_AFTER;
                    break;
                }
                else{
                    int arm_deg_check;
                    arm_deg_check=(int)moter->GetArmDegree();
                    if(arm_deg_check <= 85){
                        set_arm_check=moter->SetArm(60);
                        state=SHOT_AFTER;
                        break;
                    }
                }
                cam1_ball_deg_range = 20;
                ChangeCam(CAM_2);
                state=BALL_SEARCH;   
                break;      
            
            //-------------------------------------------------
            //Error
            //-------------------------------------------------
            default:
                return -1;
                break;
        }
        return 0;
}


int Robot::State(void){
    return state;
    //return EXIT;
}
