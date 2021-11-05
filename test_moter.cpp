#include "moter.h"
#include "com.h"

int main(void){
	Moter m;
	char c=Func::KeyState();
	do {
		c=Func::KeyState();
		if(c=='a')
			m.WheelLeft(10);
		else if(c=='d')
			m.WheelRight(10);
		else if(c=='r')
			m.TimerWheel(10,10,3000);
		else if(c=='e')
			m.TimerWheel(10,10,1000);
		else if(c=='f')
			m.TimerStop();
		m.Update();
			
	}while(c!='q');
	
	return 0;
}
