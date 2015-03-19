#include <QCoreApplication>
#include <opencv.hpp>

#include <TOFLib/include/tofmanager.h>
#include "capture.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    namedWindow("phase");
    namedWindow("Confidence");
    namedWindow("ambient");
    namedWindow("frame");
    TOFManager* manager = new TOFManager();
//    VoxelCaptureDevice *capDev = new VoxelCaptureDevice(240, 320, manager->getDeviceNumber());
    VoxelCaptureDevice *capDev = new VoxelCaptureDevice(240, 320, 1);
//    manager->getDevice()->setCaptureDevice(capDev);
    while(1){
        struct captureData info = capDev->capture();
//        Mat* phase = info.phaseData;
        imshow("phase", info.phaseData);
        imshow("Confidence", info.confidenceData);
        imshow("ambient", info.ambientData);
        imshow("frame", info.frame);
//        sleepFunction(40);
        waitKey(30);
    }

//    VideoCapture cap(0);
//    Mat test;
//    if(cap.isOpened()){
//        while(1){
//            if(!cap.read(test))
//            {
//                cout<<"fail"<<endl;
//                return -1;
//            }
//            imshow("test",test);
//            waitKey(30);
//        }
//    }else{
//        return -1;
//    }

}
