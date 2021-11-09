#include "ballFinder.h"
#include <math.h>
#include <iostream>

//#define BF_LOG


BallFinder::BallFinder(Camera *cp,int parameter,int height_distance,int depth_distance,int camera_degree)
{
    this->cp = cp;
    this->height_distance = height_distance;
    this->depth_distance = depth_distance;
    this->camera_radian = (float)camera_degree*3.14f/180.0f;
    param=100*parameter;
    distance=0;
    degree=0;
    //HSV filter
    {
        //HSV filter 1
        int H_MAX =   5,H_MIN =  0;
        int S_MAX = 255,S_MIN = 50;
        int V_MAX = 255,V_MIN = 50;

        //HSV filter 2
        int H_MAX2 = 180,H_MIN2 = 175;
        int S_MAX2 = 255,S_MIN2 =  50;
        int V_MAX2 = 255,V_MIN2 =  50;

        s_min2 = cv::Scalar(H_MIN2, S_MIN2, V_MIN2);
        s_max2 = cv::Scalar(H_MAX2, S_MAX2, V_MAX2);
        s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
        s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
    }
}

BallFinder::~BallFinder()
{
}

int BallFinder::GetDegree(int *pvalue){
    (*pvalue) = degree;
    return 1;
}
int BallFinder::GetDistance(int *pvalue){
    (*pvalue) = distance;
    return 1;
}

void BallFinder::Update(void){
    return;
    red = redfilter(cp->frame);
    cv::cvtColor(red,Gray,6);
    //--------- 2021.11.09 additional part -----------
    Mat thresh;
    threshold(Gray,thresh,0,255,THRESH_BINARY);
    Mat o_element;
    o_element = getStructuringElement (1, Size(9 , 9)); //circle
    Mat open;
    morphologyEx(thresh,open,MORPH_OPEN,o_element);
    //-------------------------------------------------
    double dis = Calc_dis(Gray);

    //camera local to robot rocal
    //distance=distance*cos(camera_radian);
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------
cv::Mat BallFinder::redfilter(Mat r_dst)
{
        Mat mask,mask2,mask3; //red mask
        Mat Red; //Red mask image
        Mat hsv; //hsv conv image
        
        medianBlur(r_dst,r_dst,3);
        cvtColor(r_dst, hsv, 40, 3); //hsv conv
        inRange(hsv, s_min, s_max, mask);
        inRange(hsv, s_min2, s_max2, mask2);
        mask3 = mask + mask2;
        r_dst.copyTo(Red, mask3);
        
        return Red;
}

double BallFinder::Calc_dis(Mat c_Gray)
{       
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(c_Gray, circles, cv::HOUGH_GRADIENT,1, c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                //printf("Error1_CAM%d \n",cam_no);
                return -1;
        }
        else if(circles.size() > 1)
        {
                //printf("Error2_CAM%d \n",cam_no);
                return -1;
        }
        else if(circles.size() == 1)
        {
                cv::Point center((int)circles[0][0], (int)circles[0][1]);
                int radius = (int)circles[0][2];
                cv::circle(c_Gray, center, radius, cv::Scalar(255, 0, 0), 2);
                double sum = param/radius;
                #ifdef BF_LOG
                    std::cout << radius <<std::endl;
                    std::cout << center <<std::endl;
                    std::cout << std::fixed << "distance:" << sum << std::endl;
                #endif
                return sum;
        }
        return -1;
        
}