#include "camera.h"
#include "com.h"

#define CAM_REMOTE

int main(void){
	char c;
	Camera cam1;
	Camera cam2;
	if(cam1.Open(0)==-1)
		return -1;
	if(cam2.Open(1)==-1){
		if(cam2.Open(2)==-1)
			return -1;
		}
	#ifdef CAM_REMOTE
		cam1.Update();
		cam2.Update();
		cam1.Capture("pic/cam1.jpg");
		cam2.Capture("pic/cam2.jpg");
	#else
		do{
			cam1.Update();
			cam2.Update();
			c=Func::KeyState();
			cam1.Show("cam1");
			cam2.Show("cam2");
			cv::waitKey(30);
		}while(c!='q');
	#endif
	cam1.Close();
	cam2.Close();

	return 0;
}
