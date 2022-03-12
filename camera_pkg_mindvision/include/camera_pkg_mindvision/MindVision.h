#include "CameraApi.h" //相机SDK的API头文件

//#include <ros/ros.h>
//#include <cv_bridge/cv_bridge.h>
//#include <image_transport/image_transport.h>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"


unsigned char  * g_pRgbBuffer; 


class MindVision
{
private:
    
    BOOL isAutoEXposure = 1; //1为自动 0为手动
    BOOL isAutoWB = 1;
    BOOL isAutoGain = 1;
    
    int fMinExposureTime = 10;
    int fMaxExposureTime = 100;
    
    double ManExposureTime = 800;//低曝光

public:
    int                     iCameraCounts = 1;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList;
    int                     hCamera;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    int                     iDisplayFrames = 10000;
    IplImage *iplImage = NULL;
    int                     channel=3;

    MindVision(/* args */);
    cv::Mat updateImage();
    BOOL MindVisionInit();
    BOOL isOK(int iStatus = -1);
    BOOL isOK(int iStatus , int iCameraCounts);
    void CameraSet();
    BOOL GetImageBuffer();
    void freeBuffer();
    ~MindVision();
};

MindVision::MindVision(/* args */)
{
}

MindVision::~MindVision()
{
    CameraUnInit(hCamera);
    //注意，现反初始化后再free
    free(g_pRgbBuffer);
}

BOOL MindVision::MindVisionInit()
{
    CameraSdkInit(1);
    this->iStatus = CameraEnumerateDevice(&tCameraEnumList,&iCameraCounts);
    this->iStatus = CameraInit(&tCameraEnumList,-1,-1,&hCamera);
    this->isOK(iStatus,iCameraCounts);
    iStatus = CameraInit(&tCameraEnumList,-1,-1,&hCamera);
    this->isOK(iStatus);
    CameraGetCapability(hCamera,&tCapability);
    //
    g_pRgbBuffer = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);
    CameraPlay(hCamera);
    if(tCapability.sIspCapacity.bMonoSensor)
    {
        channel=1;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_MONO8);
    }else
    {
        channel=3;
        CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_BGR8);
    }
}

void MindVision::CameraSet()
{
    if (this->isAutoEXposure == 0)
    {
        CameraSetAeState(this->hCamera,this->isAutoEXposure);
        CameraSetExposureTime(this->hCamera,this->ManExposureTime); 
    }
    else{}
    if( this->isAutoWB == 0)
    {
        CameraSetWbMode(this->hCamera,this->isAutoEXposure);
        CameraSetOnceWB(this->hCamera);
    }
    else{}
    if( this->isAutoGain == 0)
    {
        CameraSetUserClrTempGain(hCamera,10,10,400);
    }
    else{}
}

BOOL MindVision::isOK(int iStatus)
{
    std::cout << "state = " << iStatus << std::endl; 
}

BOOL MindVision::isOK(int iStatus , int iCameraCounts)
{
	printf("state = %d\n", iStatus);

	printf("count = %d\n", iCameraCounts);
    if(iCameraCounts==0){
        exit(0);
    }
}

BOOL MindVision::GetImageBuffer()
{
    return CameraGetImageBuffer(hCamera,&sFrameInfo,&pbyBuffer,10);
}

cv::Mat MindVision::updateImage()
{
	CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer,&sFrameInfo);    
	cv::Mat matImage(
		cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight), 
		sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		g_pRgbBuffer
		);
    return matImage;
}

void MindVision::freeBuffer()
{
    CameraReleaseImageBuffer(hCamera,pbyBuffer);
}