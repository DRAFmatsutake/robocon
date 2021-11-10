#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>

/*g++ -o _camera.out test_camera.cpp camera.cpp com.cpp -std=c++11
// -I /usr/include/opencv4 	-lopencv_core -lopencv_imgcodecs 
//							-lopencv_highgui -lopencv_videoio
*/
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
		cv::Mat frame;
		
	private:	
		cv::VideoCapture cam;
};
