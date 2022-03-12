#include "MindVision.h"

int main(int argc, char const *argv[])
{
    MindVision *cameraMindVision = new MindVision();

    cameraMindVision->MindVisionInit();
    cameraMindVision->CameraSet();

    while (true)
    {
       if(cameraMindVision->GetImageBuffer() == CAMERA_STATUS_SUCCESS)
       {
            cv::Mat image = cameraMindVision->updateImage();
           	cv::imshow("Opencv Demo", image);
            cv::waitKey(1);
            cameraMindVision->freeBuffer();
       }
    }
    delete cameraMindVision;
    return 0;
}
