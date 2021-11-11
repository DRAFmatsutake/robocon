#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "com.h"

namespace Func{
    char KeyState(void){
        struct termios oldt, newt;
        int ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        if (ch != EOF) {
            //ungetc(ch, stdin);
            return ch;
        }
        return '\0';
    }
}

Timer::Timer(){
    state=-1;
    counter=0;
    CLOCKS_PER_MSEC=1000;
}
void Timer::SetTime(int _time){
    counter=_time*CLOCKS_PER_MSEC;
    state=0;
    gettimeofday(&now,NULL);
}
void Timer::Update(){
    struct timeval t;
    gettimeofday(&t,NULL);
    long diff=(t.tv_sec-now.tv_sec)*1000000+(t.tv_usec-now.tv_usec);
    if(state==0){
        counter-=diff;
        if(counter<=0){
            state=1;}
    }
    now=t;
}
int Timer::State(){return state;}
void Timer::DropFlug(){state=-1;}
