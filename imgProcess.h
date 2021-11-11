#pragma once
#include "camera.h"
#include "objectPosition.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class ImgProcess
{
public:
    ImgProcess(Camera *pc,BallPosition *bp,PolePosition *pp,int parameter);
    ~ImgProcess();
    void Update(void);
    void SetCameraPosition(int height_distance,int depth_distance,int camera_degree);
    CameraPosition GetCameraPosition(void);

private:
    Camera *pc;
    BallPosition *bp;
    PolePosition *pp;
    int param;
    CameraPosition camPos;
    //---------------------------
    //matrix
    Mat red;
    Mat Gray;
    Mat Canny;
    Mat thresh;
    Mat o_element;
    Mat open;
    //HSV filter Scalar
    cv::Scalar s_min;
    cv::Scalar s_max;
    cv::Scalar s_min2;
    cv::Scalar s_max2;
    //ball parameter
    const int b_sens = 10;
    const int b_canny = 200;
    const int b_min = 1;
    const int b_max = 150;

    //function
    cv::Mat redfilter(Mat r_dst);
    cv::Mat ball_mask(Mat c_Gray);
    double Calc_dis(Mat c_Gray);
    double Calc_rad(Mat c_Gray);
};
