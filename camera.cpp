#include "camera.h"

Camera::Camera(){}
Camera::~Camera(){
	printf("disconnect camera\n");
	Close();
}

int Camera::Open(int device){
	cam.open(device);
	if (!cam.isOpened()){
		printf("camera is undifinded.(device: %d )\n",device);
        return -1;
    }
	//camera resize
	cam.set(cv::CAP_PROP_FRAME_WIDTH,640);
	cam.set(cv::CAP_PROP_FRAME_HEIGHT,480);
	cam.set(cv::CAP_PROP_BRIGHTNESS ,50);
	cam.set(cv::CAP_PROP_CONTRAST ,50);
	cam.set(cv::CAP_PROP_SATURATION,0);
	cam.set(cv::CAP_PROP_HUE,0);
	cam.set(cv::CAP_PROP_EXPOSURE,100);
	cam.set(cv::CAP_PROP_FPS,30);
	printf("camera open success(device: %d )\n",device);
	return 0;
}

void Camera::Update(){
	cam.read(frame);
}

void Camera::Show(char *windowName){
	imshow(windowName, frame);        
	cv::waitKey (5);
}


void Camera::Capture(){
	printf("capture img\n");
	imwrite("pic/out.jpg",frame);
}

void Camera::Close(){
	cam.release();
}
