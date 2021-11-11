#include "imgProcess.h"


ImgProcess::ImgProcess(Camera *pc,BallPosition *bp,PolePosition *pp,int parameter)
{
    this->pc=pc;
    this->bp=bp;
    this->pp=pp;
    this->param=100*parameter;
    //HSV filter
    {
        //HSV filter 1
        int H_MAX =   5,H_MIN =  0;
        int S_MAX = 255,S_MIN =100;
        int V_MAX = 255,V_MIN =100;

        //HSV filter 2
        int H_MAX2 = 179,H_MIN2 = 170;
        int S_MAX2 = 255,S_MIN2 = 100;
        int V_MAX2 = 255,V_MIN2 = 100;

        s_min2 = cv::Scalar(H_MIN2, S_MIN2, V_MIN2);
        s_max2 = cv::Scalar(H_MAX2, S_MAX2, V_MAX2);
        s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
        s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
    }
}

void ImgProcess::SetCameraPosition(int height_distance,int depth_distance,int camera_degree){
    camPos.height=height_distance;
    camPos.depth=depth_distance;
    camPos.radian= (float)camera_degree*3.14f/180.0f;
}
CameraPosition ImgProcess::GetCameraPosition(void){return camPos;}

ImgProcess::~ImgProcess()
{
}

void ImgProcess::Update(void){
    bp->Reset();
    pp->Reset();
    cv::Mat _frame=(pc->frame).clone();
    //------------------------------------------------------------
    red = redfilter(_frame);
    cv::cvtColor(red,Gray,6);
    //--------- 2021.11.09 additional part -----------
    threshold(Gray,thresh,0,255,THRESH_BINARY);
    o_element = getStructuringElement (1, Size(9 , 9)); //circle
    morphologyEx(thresh,open,MORPH_OPEN,o_element);
    //-------------------------------------------------
    //double dis = Calc_dis(Gray);
    Calc_rad(open);
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------

cv::Mat ImgProcess::redfilter(Mat _r_dst)
{
        Mat _mask,_mask2,_mask3; //red mask
        Mat _Red; //Red mask image
        Mat _hsv; //hsv conv image
        
        //medianBlur(r_dst,r_dst,3);
        cvtColor(_r_dst, _hsv, 40, 3); //hsv conv
        inRange(_hsv, s_min, s_max, _mask);
        inRange(_hsv, s_min2, s_max2, _mask2);
        _mask3 = _mask + _mask2;
        _r_dst.copyTo(_Red, _mask3);
        
        return _Red;
}
double ImgProcess::Calc_rad(Mat c_Gray)
{
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(c_Gray, circles, cv::HOUGH_GRADIENT,1, c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                return -1;
        }
        //else if(circles.size() > 1)
        //{
                //printf("Error2_CAM%d \n",cam_no);
                //return -1;
        //}
        else //if(circles.size() == 2)
        {
                cv::Point center((int)circles[0][0], (int)circles[0][1]);
                //cv::Point center2(340, 240);
                int radius = (int)circles[0][2];
                
                cv::circle(c_Gray, center, radius, cv::Scalar(255, 0, 0), 2);
                //cv::circle(c_Gray, center2, 5, cv::Scalar(255, 0, 0), 2);
                //std::cout << radius <<std::endl;
                //std::cout << center <<std::endl;
                double sum = param/radius;
                //std::cout<< std::fixed << "CAM" <<"-distance:" << sum << std::endl;
                //imshow("Result",c_Gray);
                int x= (int)circles[0][0];
                x = (x-320)/10;
                int y= (int)circles[0][1];
                y = (y-240)/10;
                //printf("X-GAP:%d\n",x);
                bp->Set(x*10,y*10,sum*10);
        }
        return -1;
}

//----------------------------------------------------------------------------------------------------
//                                             not using
//----------------------------------------------------------------------------------------------------

double ImgProcess::Calc_dis(Mat c_Gray)
{       
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(c_Gray, circles, cv::HOUGH_GRADIENT,1, c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                //printf("Error1_CAM%d \n",cam_no);
                return -1;
        }
        /*else if(circles.size() > 1)
        {
                //printf("Error2_CAM%d \n",cam_no);
                return -1;
        }*/
        else //if(circles.size() == 1)
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

cv::Mat ImgProcess::ball_mask(Mat c_Gray) //make no-ball image
{       
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(c_Gray, circles, cv::HOUGH_GRADIENT,1, c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                //printf("Error1_CAM%d \n",cam_no);
                return c_Gray;
        }
        else if(circles.size() > 1)
        {
                //printf("Error2_CAM%d \n",cam_no);
                return c_Gray;
        }
        else if(circles.size() == 1)
        {
                cv::Point center((int)circles[0][0], (int)circles[0][1]);
                int radius = (int)circles[0][2];
                cv::circle(c_Gray, center, radius, cv::Scalar(0, 0, 0), 100);
                //std::cout << radius <<std::endl;
                //std::cout << center <<std::endl;
                //double sum =100*13/radius;  //ELECOM_CAM
                //double sum =100*5/radius;       //raspicam
                //std::cout<< std::fixed << "CAM" << cam_no <<"-distance:" << sum << std::endl;
                //imshow("Result",c_Gray);
                return c_Gray;
        }
        return c_Gray;
        
}