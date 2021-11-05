#include <stdio.h>
#include <iostream>
#include <unistd.h>          // sleep()関数を使うのに必要
#include <wiringPi.h>
#include "serial.h"

int main(void){
	Serial test(5,5,'@','$');
	test.SetRingSize(4);
	
	//シリアル通信
	if(test.Open("/dev/ttyACM0",9600)==-1)
		return 0;
	char data[6]="abcde";
	test.Send(data);
	char data2[6]="fghij";
	test.Send(data2);
	char data3[6]="klmno";
	test.Send(data3);
	char data4[6]="pqrst";
	test.Send(data4);
	sleep(2);
	test.Update();
	while(test.Read(data)==1)
		printf("%s\n",data);
	test.Close();
	return 0;
}
