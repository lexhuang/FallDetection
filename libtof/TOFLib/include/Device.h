#pragma once
#ifndef DEVICE_H
#define DEVICE_H

#include "regProg.h"
//#include "capture.h"

class Device
{
public:
	char *varName;
	RegProgrammer *regProgrammer;
//	CaptureDevice *captureDevice;

	Device();
	void setProgrammer(RegProgrammer *regProg);
//	void setCaptureDevice(CaptureDevice *capDev);
	void load(char *fileName);
	void loadGUI();
	void release();

};

class VoxelDevice : public Device
{
public:
	void setTestModeEnable(int val);
	void setBlkHeaderEn(int val);
	void setOpCsPolarity(int val);
	void setFbReadyEn(int val);
	void setRampPat(int val);
	void setAmplitudeScale(int val);
	void setFrequencyScale(int val);
	void setPixCntMax(int val);
	void setSubFrames(int val);
	void setEasyConfEn(int val);
	void setIllumPolarity(int val);
	void setTgEn(int val);
	void setConfidenceThreshold(int val);
	bool setIntegrationDutyCycle(int val);
	void setPhaseOffset(int val);
	void setModPS1(int val);
	void setModPLLUpdate(int val);
};

#endif // DEVICE_H
