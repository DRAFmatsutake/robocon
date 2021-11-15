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
			//wheel power stnc
			void PowerSync(char lparam,char rparam);
			//moter value -127 ~ 127 and -128 is stio
			//value -127 ~ 127
			void Wheel(char left,char right);
			//wheel stop
			void WheelStop();
			//value -127 ~ 127,time[ms]
			// can't execute other wheel when to end wheel moveing
			void TimerWheel(char left,char right,int time);
			//state of timerwheel
			bool TimerWheelState(void);
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
			//get arm degree
			char GetArmDegree(void);
		private:
			Serial *sr;
			void SerialReceive(void);
			char AddCharValue(char a,char b);
			//value -127 ~ 127
			void _WheelRight(char value);
			//value -127 ~ 127
			void _WheelLeft (char value);
			const char R_MOTER	= 0b00000010;
			const char L_MOTER	= 0b00000001;
			const char SETARM	= 0b00000011;
			const char SHOOT	= 0b00000100;
			Timer timer;

			//sync data
			char lparam;
			char rparam;

			//local data
			char loc_rmoter;
			char loc_lmoter;
			char loc_arm_angre;

			//receive data
			char rec_rmoter;
			char rec_lmoter;
			char rec_arm_angre;
			bool rec_can_armup;
};
