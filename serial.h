#pragma once

//koteityou
class Serial{
	public:
		Serial(
			int sendDataSize_,
			int readDataSize_,
			char header_,
			char footer_
		);
		~Serial();
		//------------------------
		// Initialize and Finalize
		//------------------------
		int Open(const char* device,int baud);
		void SetRingSize(int n);
		void Close(void);

		//------------------------
		// Nomal
		//------------------------
		void Send(char *data);
		int Read(char *data);
		void Update(void);
		
	private:
		int fd;
		int sendDataSize;
		int readDataSize;
		char header;
		char footer;

		//data buff
		char *dataBuff;
		int dataBuffSize;
		int dataCount;

		//ring buffer
		void RingAddData();
		int ringSize;
		int ringCount;
		int ringTop;
		int ringButtom;
		char* ringBuff;
		
		void Encode(char in,char *tout,char *bout);
		void Decode(char tin,char bin,char *out);
};
