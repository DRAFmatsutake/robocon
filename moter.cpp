#include <stdio.h>
#include <unistd.h>     // sleep()
#include "serial.h"
#include "moter.h"
#include "com.h"
#include <math.h>

//#define MOTER_SET_LOG
//#define MOTER_SERIAL_RECEIVE_LOG

Moter::Moter(){
		//serial connect
		sr=new Serial(2,4,'@','$');
		sr->SetRingSize(5);
		sr->Open("/dev/ttyACM0",9600);
		//sr->Open("/dev/USB0",9600);

		lparam=0;
		rparam=0;
}
Moter::~Moter(){
	sr->Close();
	delete(sr);
}

void Moter::PowerSync(char lparam,char rparam){
	this->lparam=lparam;
	this->rparam=rparam;
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

bool Moter::TimerWheelState(void){
	if(timer.State()==-1)
		return false;
	else
		return true;
}

void Moter::TimerStop(){
	timer.DropFlug();
}

char Moter::AddCharValue(char a,char b){
	int _a=a,_b=b;
	if(_a>128)_a=_a-256;
	if(_b>128)_b=_b-256;
	int ans=_a+_b;
	ans=(ans>128)?128:ans;
	ans=(ans<-127)?-127:ans;
	return (char)ans;
	}

void Moter::WheelRight(char value){
	//value=AddCharValue(value,rparam);
	if(timer.State()==0)return;
	loc_rmoter=value;
	if(value==rec_rmoter)return;
	char data[2]={R_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("wheelRight set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}
void Moter::_WheelRight(char value){
	//value=AddCharValue(value,rparam);
	char data[2]={R_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("_wheelRight set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}
void Moter::WheelLeft (char value){
	//value=AddCharValue(value,lparam);
	if(timer.State()==0)return;
	loc_lmoter=value;
	if(value==rec_lmoter)return;
	char data[2]={L_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("wheelLeft set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}void Moter::_WheelLeft (char value){
	//value=AddCharValue(value,lparam);
	char data[2]={L_MOTER,value};
	#ifdef MOTER_SET_LOG
		printf("_wheelLeft set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
}

int Moter::SetArm(char value){
	if(rec_can_armup==false)return 0;
	if(value==rec_arm_angre)return 1;
	char data[2]={SETARM,value};
	#ifdef MOTER_SET_LOG
		printf("arm set:%x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
	loc_arm_angre=value;
	return 1;
}


int Moter::ArmShot(void){
	if(rec_arm_angre==0){
		printf("ArmShot, 0\n");
		return 0;
	}
	char data[2]={SHOOT,0};
	#ifdef MOTER_SET_LOG
		printf("shot now %x %x\n",data[0],data[1]);
	#endif
	sr->Send(data);
	printf("ArmShot, 1\n");
	return 1;
}


//get arm degree
char Moter::GetArmDegree(void){
	return rec_arm_angre;
}

void Moter::SerialReceive(void){

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
	return;
}

void Moter::Update(void){
	SerialReceive();
	int _counter=0;
		//printf("%d -> %d    ",rec_lmoter,loc_lmoter);
		//printf("%d -> %d    \n",rec_rmoter,loc_rmoter);
	while(_counter<100&&(loc_lmoter!=rec_lmoter||loc_rmoter!=rec_rmoter)){
		if(loc_lmoter!=rec_lmoter)
			_WheelLeft(loc_lmoter);
		if(loc_rmoter!=rec_rmoter)
			_WheelRight(loc_rmoter);
		usleep(5000);
		SerialReceive();
		_counter++;
		//printf("%d -> %d    ",rec_lmoter,loc_lmoter);
		//printf("%d -> %d    \n",rec_rmoter,loc_rmoter);
	}
	//if(_counter==100)
	//	printf("wheel send data count over\n");

	//timer
	timer.Update();
	if(timer.State()==1){
		WheelStop();
		timer.DropFlug();
	}
}
