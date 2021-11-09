#pragma once
#include "camera.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc.hpp>

//Ball serch

using namespace cv;

class BallFinder
{
public:
    //parameter pixel size?
    //height_distance up=+ down=-
    //depth_distance front=+ back=-
    //camera_degree front = 0 up=+ down=-
    BallFinder(Camera *cp,int parameter,int height_distance,int depth_distance,int camera_degree);
    ~BallFinder();
    void Update(void);
    int GetDistance(int *pvalue);
    int GetDegree(int *pvalue);
private:
    Camera *cp;
    int height_distance;
    int depth_distance;
    float camera_radian;

    int distance;
    int degree;

    int param;

    //---------------------------
    //
    Mat red;
    Mat Gray;
    Mat Canny;
    //HSV filter Scalar
    cv::Scalar s_min;
    cv::Scalar s_max;
    cv::Scalar s_min2;
    cv::Scalar s_max2;
    //ball parameter
    const int b_sens = 15;
    const int b_canny = 200;
    const int b_min = 10;
    const int b_max = 80;

    //function
    cv::Mat redfilter(Mat r_dst);
    double Calc_dis(Mat c_Gray);
};

