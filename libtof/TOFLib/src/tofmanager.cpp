#include "tofmanager.h"

static const int FRE_DATA[] = {14.4, 16.0, 18.0, 20.5, 24.0, 28.8, 36.0, 48.0};
const int* TOFManager::FREQUENCY = FRE_DATA;

VoxelDevice *TOFManager::getDevice() const
{
    return device;
}

void TOFManager::setDevice(VoxelDevice *value)
{
    device = value;
}


int TOFManager::getDeviceNumber() const
{
    return this->deviceAddr[deviceNumber];
}

void TOFManager::setDeviceNumber(int value)
{
    deviceNumber = value;
}

TOFManager::TOFManager()
{
    isDeviceOpen =false;
    this->device = new VoxelDevice();
    this->deviceNumber = 1;
    majorVersion = 0;
    minorVersion = 0;
    this->deviceCount = 0;
    previousSubFrames = 4;

    this->fwInit();

    this->populateDeviceList();
    VoxelI2CRegProgrammer *regProg = new VoxelI2CRegProgrammer(deviceAddr[this->deviceNumber], deviceCount);
    this->device->setProgrammer(regProg);
    tpsDevice = new TPS659122RegProgrammer();

    device->regProgrammer->selectDevice(deviceAddr[deviceNumber], deviceCount);
    reInit();
}

TOFManager::~TOFManager(){
    if (isDeviceOpen) {
        device->release();
    }
}

void TOFManager::populateDeviceList() {
    int j, fd;
    int devCounter = 0;
    char devName[255];
    for (j = 0; j < 64; j++) {
        sprintf(devName, "/dev/video%d", j);
        fd = open(devName, O_RDWR);
        if (fd == -1) {
        }
        else {
            deviceAddr[devCounter] = j;
            devCounter += 1;
        }
    }
    deviceCount = devCounter;
}

void TOFManager::fwInit()
{
    string tempAddr;
//    tempAddr.append(QDir::currentPath().toLocal8Bit().data());
//    tempAddr.append("/OPT9220_0v27.tip");
    tempAddr.append("/sdcard/OPT9220_0v27.tip");
    FW_DOWNLOADER::sendsp(strdup(tempAddr.c_str()));
    sleepFunction(2000);
    this->enumerateCameras();
    printf("send tip");
}

static int xioctl(int fd, int request, void *arg) {
    int r;
    do {
        r = ioctl(fd, request, arg);
    } while (-1 == r && EINTR == errno);
    return r;
}

struct v4l2_capability print_caps(int fd) {
    struct v4l2_capability caps = {};
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps)) {
        perror("Querying Capabilities");
        return caps;
    }
    return caps;
}

void TOFManager::enumerateCameras()
{
    //cameraSelect->combo->clear();
    int i, j;
    char tempStr[255];
    struct v4l2_capability retDevName;
    char devName[255];
    int temp = 1;
    int devCounter = 0;
    int fd;
    for (j = 0; j < 64; j++) {
        sprintf(devName, "/dev/video%d", j);
        fd = open(devName, O_RDWR);
        if (fd == -1) {
        }
        else {
            retDevName = print_caps(fd);
            sprintf(tempStr, "%d: %s", j, retDevName.card);
            //cameraSelect->combo->addItem(tempStr);
            deviceAddr[devCounter] = j;
            devCounter += 1;
        }
    }
}

void TOFManager::voxelInit()
{
    VoxelI2CRegProgrammer *tempProg = new VoxelI2CRegProgrammer(deviceAddr[deviceNumber], deviceCount);
    device->setProgrammer(tempProg);
    tpsDevice = new TPS659122RegProgrammer();

    /*
     * Make sure to start with a lower illum power
     * in case the range register is not properly set
     * and then increase it just after
     */
    tpsDevice->writeReg(0x0E, 0x14);
    tpsDevice->writeReg(0x0F, 0xFF);
    tpsDevice->writeReg(0x0D, 0x54);
    tpsDevice->writeReg(0x0E, 0x94);

    device->setRampPat(3);
    device->setBlkHeaderEn(1);
    device->setOpCsPolarity(1);
    device->setFbReadyEn(1);
    device->setAmplitudeScale(2);
    device->setFrequencyScale(25);
    device->setPixCntMax(120000);
    device->setEasyConfEn(1);
    // FIXME: This inverts the illum_en polarity
    // and should be done only on the rev 2p2 and newer boards
    device->setIllumPolarity(0);
    device->setConfidenceThreshold(1);
    int versionReg = device->regProgrammer->readReg(1);
    majorVersion = (versionReg & 0xFF00) >> 8;
    minorVersion = versionReg & 0xFF;
    if(majorVersion == 85 && minorVersion == 170) {
        majorVersion = 0;
        minorVersion = 0;
    }
    this->setIntegrationTime(30);
    device->setTgEn(1);
    this->setModulationFrequency(TOFManager::FREQUENCY[2]);
    this->setSubFrames(4);
    device->setPhaseOffset(0);

    device->setTestModeEnable(0);
    device->setIllumPolarity(1500);
    isDeviceOpen = true;
}

void TOFManager::reInit() {
    /*
     * If device is open release it
     * Also call voxelInit
    */
    if (isDeviceOpen) {
        device->release();
    }
    this->voxelInit();
}

void TOFManager::setSubFrames(int subframe)  //1,2,4
{
    int currentPixCntMax, writeVal;
    currentPixCntMax = (device->regProgrammer->readReg(PIX_CNT_MAX) & 0x3fffff);

    device->setSubFrames(subframe);
    writeVal = (previousSubFrames*currentPixCntMax) / subframe;
    device->setPixCntMax(writeVal);
    previousSubFrames = subframe;
    setIntegrationTime(30);
}

void TOFManager::setModulationFrequency(int frequency)
{
    device->setModPS1(frequency);
    device->setModPLLUpdate(1);
    sleepFunction(50);
    device->setModPLLUpdate(0);
    sleepFunction(50);
    // Delay maybe not required
}

void TOFManager::setIntegrationTime(int value)  //default 30
{
    bool regOverflow;
    regOverflow = device->setIntegrationDutyCycle(64 * value / 100);
}

void TOFManager::setIllumPower(int value)
{
    int setReg;

    if (value >= 3650)
        setReg = 128 + 0x3F;
    else if (value < 500)
        setReg = 128;
    else
        setReg = 128 + (value - 500) / 50;

    tpsDevice->writeReg(14, setReg);
}

void TOFManager::setPhaseOffset(int value)
{
    if (value < 0)
        value += 4096;
    device->setPhaseOffset(value);
}
