#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <thread>
#include <mutex>

class Camera{
	public:
		Camera();
		~Camera();
		int Open(int device);
		void Close();
		void Show(const char *windowName);
		void Update();
		void Capture();
		void Capture(const char *path);
		cv::Mat GetClone();
		
	private:
		void cam_thread();
		cv::VideoCapture cam;
		std::thread th;
		bool loop;
		int state;
		cv::Mat frame;
		cv::Mat _frame;
		std::mutex mtx;
};
