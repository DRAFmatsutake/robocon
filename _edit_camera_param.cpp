#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc.hpp>

int main(void){
    
	cv::Mat frame;
	cv::VideoCapture cam;
    int device=1;
    //HSV filter Scalar
    cv::Scalar s_min;
    cv::Scalar s_max;
    cv::Scalar s_min2;
    cv::Scalar s_max2;
    
    //HSV filter
    {
        //HSV red filter 1
        //int H_MAX =   5,H_MIN =  0;
        int H_MAX =   3,H_MIN =  0;
        int S_MAX = 255,S_MIN = 50;
        int V_MAX = 255,V_MIN = 50;

        //HSV red filter 2
        //int H_MAX2 = 179,H_MIN2 = 170;
        int H_MAX2 = 179,H_MIN2 = 175;

        s_min2 = cv::Scalar(H_MIN2, S_MIN, V_MIN);
        s_max2 = cv::Scalar(H_MAX2, S_MAX, V_MAX);
        s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
        s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
    }

	cam.open(device);
	if (!cam.isOpened()){
		printf("camera is undifinded.(device: %d )\n",device);
        return -1;
    }
	//camera resize
	
	cam.set(cv::CAP_PROP_FRAME_WIDTH,640);
	cam.set(cv::CAP_PROP_FRAME_HEIGHT,480);
	cam.set(cv::CAP_PROP_BRIGHTNESS ,-20);
	//cam.set(cv::CAP_PROP_CONTRAST ,0);
	//cam.set(cv::CAP_PROP_SATURATION,-1);
	//cam.set(cv::CAP_PROP_HUE,-1);
	//cam.set(cv::CAP_PROP_EXPOSURE,1000);
	cam.set(cv::CAP_PROP_FPS,30);
	cam.read(frame);
    
	cv::imwrite("pic/out_cam.jpg",frame);
    
    cv::Mat _mask,_mask2,_mask3; //red mask
    cv::Mat _Red; //Red mask image
    cv::Mat _hsv; //hsv conv image
    
    cv::medianBlur(frame,frame,3);
    cv::cvtColor(frame, _hsv, 40, 3); //hsv conv
    cv::inRange(_hsv, s_min, s_max, _mask);
    cv::inRange(_hsv, s_min2, s_max2, _mask2);
    _mask3 = _mask + _mask2;
    frame.copyTo(_Red, _mask3);
    
	cv::imwrite("pic/out_hsv.jpg",_Red);
    
	cam.release();
	return 0;
}