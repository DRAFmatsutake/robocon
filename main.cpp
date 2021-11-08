#include "robot.h"
#include <unistd.h>     // sleep()

int main(void){
    Robot robot;
    robot.Init();
    while(robot.Run()==0);
    robot.Deinit();
    sleep(3);
    return 0;
}