/*******************************************************
*
*  Copyright (c) 2014 Texas Instruments Incorporated
*
*******************************************************/

#include "Device.h"
#include "OPT9220_regmap.h"

Device::Device()
{
}

void Device::setProgrammer(RegProgrammer *regProg)
{
	regProgrammer = regProg;
}

//void Device::setCaptureDevice(CaptureDevice *capDev)
//{
//	captureDevice = capDev;
//}

void Device::release()
{
//	if (captureDevice) {
//		captureDevice->release();
//	}
//	else {
//		logMessage("No capture device\n");
//	}
}

void VoxelDevice::setTestModeEnable(int val) {
	regProgrammer->setReg(49, val, 0, 0);
}
void VoxelDevice::setBlkHeaderEn(int val) {
	regProgrammer->setReg(BLK_HEADER_EN, val, BLK_HEADER_EN_MSB, BLK_HEADER_EN_LSB);
}
void VoxelDevice::setOpCsPolarity(int val) {
	regProgrammer->setReg(OP_CS_POL, val, OP_CS_POL_MSB, OP_CS_POL_LSB);
}
void VoxelDevice::setFbReadyEn(int val) {
	regProgrammer->setReg(FB_READY_EN, val, FB_READY_EN_MSB, FB_READY_EN_LSB);
}
void VoxelDevice::setRampPat(int val) {
	regProgrammer->setReg(49, val, 3, 1);
}
void VoxelDevice::setAmplitudeScale(int val) {
	regProgrammer->setReg(49, val, 8, 4);
}
void VoxelDevice::setFrequencyScale(int val) {
	regProgrammer->setReg(49, val, 13, 9);
}
void VoxelDevice::setPixCntMax(int val) {
	regProgrammer->setReg(PIX_CNT_MAX, val, PIX_CNT_MAX_MSB, PIX_CNT_MAX_LSB);
}
void VoxelDevice::setSubFrames(int val) {
	regProgrammer->setReg(SUB_FRAME_CNT_MAX, val, SUB_FRAME_CNT_MAX_MSB, SUB_FRAME_CNT_MAX_LSB);
}
void VoxelDevice::setEasyConfEn(int val) {
	regProgrammer->setReg(EASY_CONF_EN, val, EASY_CONF_EN_MSB, EASY_CONF_EN_LSB);
}
void VoxelDevice::setIllumPolarity(int val) {
	regProgrammer->setReg(ILLUM_EN_POL, val, ILLUM_EN_POL_MSB, ILLUM_EN_POL_LSB);
}
void VoxelDevice::setTgEn(int val) {
	regProgrammer->setReg(TG_EN, val, TG_EN_MSB, TG_EN_LSB);
}
void VoxelDevice::setConfidenceThreshold(int val) {
	regProgrammer->setReg(CONFIDENCE_THRESHOLD, val, CONFIDENCE_THRESHOLD_MSB, CONFIDENCE_THRESHOLD_LSB);
}
bool VoxelDevice::setIntegrationDutyCycle(int val) {
	int overflowReg, overflowValue;
	bool regOverflow;
	overflowReg = regProgrammer->readReg(INTG_DUTY_CYCLE_SET_FAILED);
	overflowValue = (overflowReg >> INTG_DUTY_CYCLE_SET_FAILED_MSB) & 0x01;
	if (overflowValue == 1 && overflowReg != 0xcc55aa) {
		regOverflow = true;
	}
	else {
		regOverflow = false;
	}
	regProgrammer->setReg(INTG_DUTY_CYCLE, val, INTG_DUTY_CYCLE_MSB, INTG_DUTY_CYCLE_LSB);
	return regOverflow;
}

void VoxelDevice::setPhaseOffset(int val) {
	regProgrammer->setReg(PHASE_CORR_1, val, PHASE_CORR_1_MSB, PHASE_CORR_1_LSB);
}

void VoxelDevice::setModPS1(int val)
{
	regProgrammer->setReg(MOD_PS1, val, MOD_PS1_MSB, MOD_PS1_LSB);
}

void VoxelDevice::setModPLLUpdate(int val)
{
	regProgrammer->setReg(MOD_PLL_UPDATE, val, MOD_PLL_UPDATE_MSB, MOD_PLL_UPDATE_LSB);
}
