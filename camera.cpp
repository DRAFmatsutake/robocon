#include "camera.h"

Camera::Camera(){
	loop=false;
	state=0;
}
Camera::~Camera(){
	Close();
	printf("disconnected camera\n");
}

cv::Mat Camera::GetClone(){
	mtx.lock();
	_frame=frame.clone();
	mtx.unlock();
	return _frame;
}

int Camera::Open(int device){
	if(state==1){
		printf("this camera instance is already opened\n");
	}
	cam.open(device);
	if (!cam.isOpened()){
		printf("camera is undifinded.(device: %d )\n",device);
		state= 0;
        return -1;
    }
	//camera resize
	
	cam.set(cv::CAP_PROP_FRAME_WIDTH,640);
	cam.set(cv::CAP_PROP_FRAME_HEIGHT,480);
	//cam.set(cv::CAP_PROP_BRIGHTNESS ,0);
	//cam.set(cv::CAP_PROP_CONTRAST ,0);
	//cam.set(cv::CAP_PROP_SATURATION,-1);
	//cam.set(cv::CAP_PROP_HUE,-1);
	//cam.set(cv::CAP_PROP_EXPOSURE,1000);
	cam.set(cv::CAP_PROP_FPS,30);
	printf("camera open success(device: %d )\n",device);
	state=1;
	loop=true;
	th=std::thread(&Camera::cam_thread,this);
	return 0;
}

void Camera::Update(){
}
void Camera::cam_thread(){
	printf("start cam thead\n");
	while(loop){
		if(cam.grab()){
			mtx.lock();
			cam.retrieve(frame);
			mtx.unlock();
		}
	}
	printf("end cam thead\n");
}

void Camera::Show(const char *windowName){
	cv::imshow(windowName, GetClone());        
	cv::waitKey (5);
}


void Camera::Capture(){
	Capture("pic/out.jpg");
}


void Camera::Capture(const char *path){
	printf("capture img\n");
	imwrite(path,GetClone());
}

void Camera::Close(){
	if(state==1){
		loop=false;
		state=0;
		th.join();
		cam.release();
	}
}
