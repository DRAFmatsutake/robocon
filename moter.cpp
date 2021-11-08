#include <stdio.h>
#include <unistd.h>     // sleep()
#include "serial.h"
#include "moter.h"
#include "com.h"

//#define MOTER_SET_LOG
//#define MOTER_SERIAL_RECEIVE_LOG

Moter::Moter(){
		//serial connect
		sr=new Serial(2,4,'@','$');
		sr->SetRingSize(5);
		sr->Open("/dev/ttyACM0",9600);
		//sr->Open("/dev/USB0",9600);
}
Moter::~Moter(){
	sr->Close();
	delete(sr);
}

void Moter::Wheel(char left,char right){
	WheelLeft(left);
	WheelRight(right);
}
void Moter::WheelStop(void){
	Wheel(-128,-128);
}
void Moter::TimerWheel(char left,char right,int time){
	Wheel(left,right);
	timer.SetTime(time);
}

void Moter::TimerStop(){
	timer.DropFlug();
}

void Moter::WheelRight(char value){
	if(timer.State()==0)return;
	if(value==rec_rmoter)return;
	char data[2]={R_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("wheelRight set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}
void Moter::WheelLeft (char value){
	if(timer.State()==0)return;
	if(value==rec_lmoter)return;
	char data[2]={L_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("wheelLeft set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}

int Moter::SetArm(char value){
	if(rec_can_armup==false)return 0;
	if(value==rec_arm_angre)return 0;
	char data[2]={SETARM,value};
	#ifdef MOTER_SET_LOG
		printf("arm set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
	return 1;
}


int Moter::ArmShot(void){
	if(rec_arm_angre==0)
		return 1;
	char data[2]={SHOOT,0};
	#ifdef MOTER_SET_LOG
		printf("shot now %x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
	return 1;
}

void Moter::Update(void){

	//serial
	sr->Update();
	{
		char data[4]={0,0,0,0};
		int count=0;
		while(-1!=sr->Read(data)){count++;}
		if(count!=0){
			rec_lmoter=data[0];
			rec_rmoter=data[1];
			rec_arm_angre=data[2];
			rec_can_armup=(data[3]&0x01!=0)?true:false;
			#ifdef MOTER_SERIAL_RECEIVE_LOG
				printf("L:%x R:%x arm:%x canup:%s\n",rec_lmoter,rec_rmoter,rec_arm_angre,(rec_can_armup)?"true":"false");
			#endif
		}
	}

	//timer
	timer.Update();
	if(timer.State()==1){
		Wheel(0,0);
		timer.DropFlug();
	}
}
