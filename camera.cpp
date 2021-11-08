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
	cam.set(3,cam.get(3)/2);	//width
	cam.set(4,cam.get(4)/2);	//height
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
