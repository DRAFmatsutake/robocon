#include <wiringSerial.h>
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>     // sleep()
#include "serial.h"
#include "com.h"

//#define SERIAL_LOG

Serial::Serial(
			int sendDataSize_,
			int readDataSize_,
			char header_,
			char footer_
		)
{
	sendDataSize=sendDataSize_;
	readDataSize=readDataSize_;
	header=header_;
	footer=footer_;
	fd=-1;
	dataCount=-1;
	ringBuff=NULL;
	SetRingSize(10);
	dataBuffSize=readDataSize*2;
	dataBuff=(char*)malloc(sizeof(char)*dataBuffSize);
}
Serial::~Serial(){
	free(ringBuff);
	free(dataBuff);
	Close();
}

int Serial::Open(const char* device,int baud){
	wiringPiSetup();
	fflush(stdout);
	fd = serialOpen(device,baud);
	if(fd<0){
		std::cout<<device<<" open feild"<<std::endl;
		return -1;
	}     
	else{
		std::cout<<device<<" open success"<<std::endl;
		sleep(3);
		return 0;
	}  
}

void Serial::Close(void){
	if(0<=fd){
		std::cout<<"close serial port"<<std::endl;
		serialClose(fd);
		fd=-1;
	}
}

void Serial::Send(char *data){
	if(fd<0)
		return;
	serialPutchar(fd,header);
		#ifdef SERIAL_LOG
			printf("send:%x ",header);
		#endif
	for(int i=0;i<sendDataSize;i++){
		char tout,bout;
		Encode(data[i],&tout,&bout);
		serialPutchar(fd,tout);
		serialPutchar(fd,bout);
		#ifdef SERIAL_LOG
			printf("[%x",tout);
			printf(":%x] ",bout);
		#endif
	}
	serialPutchar(fd,footer);
	#ifdef SERIAL_LOG
		printf("%x\n",footer);;
	#endif
}

int Serial::Read(char *data){
	if(ringCount==0){
		/*
		if(R_REDU)
			printf("buffer empty\n");*/
		return -1;
	}
	int tt=ringButtom*readDataSize;
	for(int i=0;i<readDataSize;i++){
		data[i]=ringBuff[tt+i];
	}
	ringCount--;
	if(ringCount!=0)
		ringButtom=(ringButtom+1)%ringSize;
	return 1;
}

void Serial::SetRingSize(int n){
	if(ringBuff!=NULL)
		free(ringBuff);
	ringSize=n;
	ringCount=0;
	ringTop=0;
	ringButtom=0;
	ringBuff=(char*)malloc(sizeof(char)*readDataSize*ringSize);
	if(ringBuff==NULL)
		printf("malloc fault\n");
}

void Serial::Decode(char tin,char bin,char *out){
	if(tin<0x41)
		tin-=0x30;
	else
		tin-=0x37;
	if(bin<0x41)
		bin-=0x30;
	else
		bin-=0x37;
	(*out)=(tin<<4)+bin;
}

void Serial::Encode(char in,char *tout,char *bout){
	(*tout)=(char)(in>>4);
	(*bout)=(char)(in&0x0F);
	if((*tout)<0xA)
		(*tout)+=0x30;
	else
		(*tout)+=0x37;
	if((*bout)<0xA)
		(*bout)+=0x30;
	else
		(*bout)+=0x37;
}

void Serial::Update(void){
	char c;
	if(fd<0)
		return;
	while(0!=serialDataAvail(fd)){
		c=serialGetchar(fd);
		#ifdef SERIAL_LOG
			printf("[%x %d]",c,dataCount);
		#endif
		if(c==header){
			dataCount=0;
		}
		else if(c==footer){
			if(dataCount==dataBuffSize){
				#ifdef SERIAL_LOG
					printf("success get signal\n");
				#endif
				RingAddData();
			}
			dataCount=-1;
		}
		else if(dataCount>=dataBuffSize){
			dataCount=-1;
		}
		else if(dataCount!=-1){
			dataBuff[dataCount]=c;
			dataCount++;
		}
	}
}

void Serial::RingAddData(void){
	if(ringCount!=0)
		ringTop=(ringTop+1)%ringSize;
	if(ringCount<ringSize)
		ringCount++;
	else
		ringButtom=(ringButtom+1)%ringSize;
	int tt=ringTop*readDataSize;
	for(int i=0;i<readDataSize;i++){
		int ii=i*2;
		char c;
		Decode(dataBuff[ii],dataBuff[ii+1],&ringBuff[tt+i]);
	}
}
