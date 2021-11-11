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
    ImgProcess(Camera *pc,BallPosition *bp,HolePosition *hp,int parameter);
    ~ImgProcess();
    void Update(void);
    void SetCameraPosition(int height_distance,int depth_distance,int camera_degree);
    CameraPosition GetCameraPosition(void);

private:
    Camera *pc;
    BallPosition *bp;
    HolePosition *hp;
    int param;
    CameraPosition camPos;

    typedef struct
    {
        int x=0;
        int y=0;
    } ImgProcess_Position;
    ImgProcess_Position _pos;

    //---------------------------
    //matrix
    cv::Mat o_element;
    cv::Mat CAM1;
    cv::Mat CAM2;

    //HSV filter Scalar
    cv::Scalar s_min;
    cv::Scalar s_max;
    cv::Scalar s_min2;
    cv::Scalar s_max2;
    
    //HSV GREEN
    cv::Scalar g_min;
    cv::Scalar g_max;
    //ball parameter
    const int b_sens = 10;
    const int b_canny = 200;
    const int b_min = 1;
    const int b_max = 150;

    //function
    cv::Mat redfilter(Mat r_dst);
    cv::Mat ball_mask(Mat c_Gray);
    double Calc_rad(Mat c_Gray);
    
    cv::Mat greenfilter(Mat _g_dst);
    int flag_dis(Mat _CAM);
    int flag_pos(Mat _CAM,int tmp);
    int hole_pos(Mat _CAM,int tmp);
};
