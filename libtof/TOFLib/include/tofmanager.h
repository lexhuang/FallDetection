#ifndef TOFMANAGER_H
#define TOFMANAGER_H

#include "Device.h"
#include "voxelfw-linux.h"
#include "OPT9220_regmap.h"

//file open for devices
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <errno.h>
#include <string>

#include <linux/videodev2.h>

//#include <qdir.h>
using namespace std;

class TOFManager
{
private:
    VoxelDevice *device;
    int deviceNumber;
    int deviceCount;
    int deviceAddr[64];
    int majorVersion;
    int minorVersion;
    bool isDeviceOpen;
    TPS659122RegProgrammer *tpsDevice;
    int previousSubFrames;
public:
    static const int* FREQUENCY;
    TOFManager();
    ~TOFManager();
    void populateDeviceList();
    void fwInit();
    void enumerateCameras();
    void voxelInit();
    VoxelDevice *getDevice() const;
    void setDevice(VoxelDevice *value);
    void reInit();
    int getDeviceNumber() const;
    void setDeviceNumber(int value);
    void setSubFrames(int subframe);
    void setModulationFrequency(int frequency);
    void setIntegrationTime(int value);
    void setIllumPower(int value);
    void setPhaseOffset(int value);


};

#endif // TOFMANAGER_H
