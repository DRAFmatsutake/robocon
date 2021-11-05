#pragma once
#include "serial.h"
#include "com.h"

class Moter{
		public:
			Moter(void);
			~Moter();
			void Update(void);
			//--------------------------
			//		set
			//--------------------------
			//moter value -127 ~ 127 and -128 is stio
			//value -127 ~ 127
			void Wheel(char left,char right);
			//wheel stop
			void WheelStop();
			//value -127 ~ 127,time[ms]
			// can't execute other wheel when to end wheel moveing
			void TimerWheel(char left,char right,int time);
			//cancel TimerWheel
			void TimerStop();
			//value -127 ~ 127
			void WheelRight(char value);
			//value -127 ~ 127
			void WheelLeft (char value);

			//value 0 ~ 120  about 0-100
			// return success:1 faild:0
			int SetArm(char value);
			//excute shot
			// return success:1 faild:0
			int ArmShot(void);

			//--------------------------
			//		get
			//--------------------------
			//int GetWheelSpeed(char *left,char*right);
			//int GetArmPower(char *value);
		private:
			Serial *sr;
			const char R_MOTER	= 0b00000010;
			const char L_MOTER	= 0b00000001;
			const char SETARM	= 0b00000011;
			const char SHOOT	= 0b00000100;
			Timer timer;

			//receive data
			char rec_rmoter;
			char rec_lmoter;
			char rec_arm_angre;
			bool rec_can_armup;
};
