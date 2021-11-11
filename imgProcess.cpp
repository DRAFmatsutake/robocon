#include "imgProcess.h"


ImgProcess::ImgProcess(Camera *pc,BallPosition *bp,HolePosition *hp,int parameter)
{
    this->pc=pc;
    this->bp=bp;
    this->hp=hp;
    this->param=100*parameter;
    //HSV filter
    {
        //HSV red filter 1
        int H_MAX =   5,H_MIN =  0;
        int S_MAX = 255,S_MIN = 50;
        int V_MAX = 255,V_MIN = 50;

        //HSV red filter 2
        int H_MAX2 = 179,H_MIN2 = 170;

        s_min2 = cv::Scalar(H_MIN2, S_MIN, V_MIN);
        s_max2 = cv::Scalar(H_MAX2, S_MAX, V_MAX);
        s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
        s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);

        //HSV green filter
        int GH_MAX = 100,GH_MIN = 30;
        int GS_MAX = 255,GS_MIN = 0;
        int GV_MAX = 255,GV_MIN = 0;
        g_min = cv::Scalar(GH_MIN, GS_MIN, GV_MIN);
        g_max = cv::Scalar(GH_MAX, GS_MAX, GV_MAX);
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
    hp->Reset();
    CAM1=(pc->frame).clone();
    CAM2=CAM1.clone();

    //ball flag search
    CAM1 = redfilter(CAM1);
    cv::cvtColor(CAM1,CAM1,6);
    threshold(CAM1,CAM1,0,255,THRESH_BINARY);
    o_element = getStructuringElement (MORPH_ELLIPSE, Size(9 , 9)); //circle
    morphologyEx(CAM1,CAM1,MORPH_OPEN,o_element);
    Calc_rad(CAM1);
    CAM1 = ball_mask(CAM1);
    int flag_distance = flag_dis(CAM1);
    int _flag_pos = flag_pos(CAM1,0);
    if(flag_distance!=-1&&_flag_pos!=-1){
            hp->SetFlagPos(_pos.x*10,_pos.y*10,flag_distance*10);
    }

    //hole search
    CAM2 = greenfilter(CAM2);
    cv::cvtColor(CAM2,CAM2,6);
    threshold(CAM2,CAM2,0,255,THRESH_BINARY);
    morphologyEx(CAM2,CAM2,MORPH_OPEN,o_element); 
}

//-------------------------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------------------------

cv::Mat ImgProcess::redfilter(Mat _r_dst)
{
        Mat _mask,_mask2,_mask3; //red mask
        Mat _Red; //Red mask image
        Mat _hsv; //hsv conv image
        
        medianBlur(_r_dst,_r_dst,3);
        cvtColor(_r_dst, _hsv, 40, 3); //hsv conv
        inRange(_hsv, s_min, s_max, _mask);
        inRange(_hsv, s_min2, s_max2, _mask2);
        _mask3 = _mask + _mask2;
        _r_dst.copyTo(_Red, _mask3);
        
        return _Red;
}
cv::Mat ImgProcess::greenfilter(Mat _g_dst) //midori wo kesu
{
        Mat mask; //green mask
        Mat green; //green mask image
        Mat hsv; //hsv conv image
        
        medianBlur(_g_dst,_g_dst,3);
        cvtColor(_g_dst, hsv, 40, 3); //hsv conv
        inRange(hsv, g_min, g_max, mask);
        bitwise_not(mask,mask);
        _g_dst.copyTo(green, mask);
        
        return green;
}

Mat ImgProcess::ball_mask(Mat _c_Gray) //boll wo kesu
{       
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(_c_Gray, circles, cv::HOUGH_GRADIENT,1, _c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                return _c_Gray;
        }
        else 
        {
                int m_low = 0;
                int m_low_id =0;
                for(int i=0;i<circles.size();++i)
                {
                        if(m_low < (int)circles[i][1])
                        {
                                m_low = (int)circles[i][1];
                                m_low_id = i;
                        }
                }
                cv::Point center((int)circles[m_low_id][0], (int)circles[m_low_id][1]);
                int radius = (int)circles[m_low_id][2];
                cv::circle(_c_Gray, center, radius, cv::Scalar(0, 0, 0), 120);
                return _c_Gray;
        }
        return _c_Gray;
        
}

double ImgProcess::Calc_rad(Mat c_Gray)
{
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(c_Gray, circles, cv::HOUGH_GRADIENT,1, c_Gray.rows/2, b_canny, b_sens,b_min,b_max);
        if(circles.size() == 0)
        {
                return -1;
        }
        else
        {
                int m_low = 0;
                int m_low_id =0;
                
                for(int i=0;i<circles.size();++i)
                {
                        if(m_low < (int)circles[i][1])
                        {
                                m_low = (int)circles[i][1];
                                m_low_id = i;
                        }
                }
                cv::Point center((int)circles[m_low_id][0], (int)circles[m_low_id][1]);
                int radius = (int)circles[m_low_id][2];


                double sum = param/radius;
                //std::cout<< std::fixed << "CAM" <<"-distance:" << sum << std::endl;
                //imshow("Result",c_Gray);
                int x= (int)circles[m_low_id][0];
                x = (x-320)/10;
                int y= (int)circles[m_low_id][1];
                y = (y-240)/10;
                //printf("X-GAP:%d\n",x);
                bp->Set(x*10,y*10,sum*10);
        }
        return -1;
}
int ImgProcess::flag_dis(Mat _CAM) //hata no tate no ookisa
{
        int height;
        int width;
        Mat lab;
        Mat l_status;
        Mat cent;
        int nLab = connectedComponentsWithStats(_CAM,lab,l_status,cent);
        if(nLab-1==0)
                return -1;
         
        
        cv::Mat dst_lab(_CAM.size(), CV_8UC3);
        
        for (int i = 1; i < nLab; ++i) {
                int *param = l_status.ptr<int>(i);

                int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
                int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
                height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
                width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
                
                cv::rectangle(dst_lab, cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0), 2);
        }
        height = 100*56/height;
        return height;
}

int ImgProcess::flag_pos(Mat _CAM,int tmp)
{
        int height;
        int width;
        int x,y;
        int h_m,w_m;
        cv::Mat lab;
        cv::Mat l_status;
        cv::Mat cent;
        int nLab = connectedComponentsWithStats(_CAM,lab,l_status,cent);
        if(nLab-1==0)
                return -1;
        cv::Mat dst_lab(_CAM.size(), CV_8UC3);
        int max_y = 480;
        int max_x = -1;
        int id;
        
        for (int i = 1; i < nLab; ++i) 
        {
                double *_param = cent.ptr<double>(i);
                x = static_cast<int>(_param[0]);
                y = static_cast<int>(_param[1]);
                if(y < max_y)
                {
                        max_y = y;
                        max_x = x;
                        id = i;
                }
        }
        cv::circle(dst_lab,cv::Point(max_x, max_y), 3, cv::Scalar(0, 0, 255), 1);
        
        _pos.x=(max_x-320)/10;
        _pos.y=(max_y-240)/10;
        return 0;
}


int ImgProcess::hole_pos(Mat _CAM,int tmp)
{
        int height;
        int width;
        cv::Mat lab;
        cv::Mat l_status;
        cv::Mat cent;
        int area,area_max = 0;
        int id;
        int y_max_tmp = 480;
        int x_max ,y_max;
        int height_max;
        int width_max;
        
        int nLab = connectedComponentsWithStats(_CAM,lab,l_status,cent);
        
        cv::Mat dst_lab(_CAM.size(), CV_8UC3);
        
        for (int i = 1; i < nLab; ++i) {
                int *_param = l_status.ptr<int>(i);

                int x = _param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
                int y = _param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
                height = _param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
                width = _param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
                area = height * width;
                if(area_max < area)
                        if(y_max_tmp > x)
                        {
                                area_max = area;
                                id = i;
                                height_max = height;
                                width_max = width; 
                                x_max = x + width/2;
                                y_max = y + height/2;
                        }
        }
        cv::rectangle(dst_lab, cv::Rect(x_max, y_max, width_max, height_max), cv::Scalar(0, 255, 0), 2);
        
        _pos.x=x_max;
        _pos.y=y_max;
        
        return 0;
}
//----------------------------------------------------------------------------------------------------
//                                             not using
//----------------------------------------------------------------------------------------------------
