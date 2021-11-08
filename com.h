#pragma once

namespace Func{
	//no wait key input
	char KeyState(void);
}

//time counter
class Timer{
	public:
		Timer();
		//set timer time [ms]
		void SetTime(int time);
		//get timer state
		//unset:-1 wait:0 cmpleate:1
		int State();
		//execute this fnction if cmpleate wait
		//change state to unset
		void DropFlug();
		//counter update
		void Update();
	private:
		int state;
		long counter;
		long now;
		int CLOCKS_PER_MSEC;
};
