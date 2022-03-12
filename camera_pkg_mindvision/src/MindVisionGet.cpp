#include "MindVision.h"
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

int main(int argc, char** argv)
{
    
    ros::init(argc,argv,"MindVisionPicture_publish");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);//为图像传输提供节点
    image_transport::Publisher pubPic = it.advertise("MindVisionCamera/LowDims", 1);
    
    MindVision *cameraMindVision = new MindVision();

    cameraMindVision->MindVisionInit();
    cameraMindVision->CameraSet();
    ros::Rate loop_rate(120);

    while (true)
    {
       if(cameraMindVision->GetImageBuffer() == CAMERA_STATUS_SUCCESS)
       {
            cv::Mat image = cameraMindVision->updateImage();
            
            sensor_msgs::ImagePtr msg  = cv_bridge::CvImage(std_msgs::Header(), "bgr8",image).toImageMsg();//mat转ros
            pubPic.publish(msg);
           	
            //cv::imshow("Opencv Demo", image);
            //cv::waitKey(1);

            cameraMindVision->freeBuffer();
       }
    }
    delete cameraMindVision;
    return 0;
}
