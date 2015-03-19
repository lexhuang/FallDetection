#pragma once

#ifndef CAPTURE_H
#define CAPTURE_H

#include "cnumpy.h"
#include "Globals.h"
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

struct captureData {
	Mat frame;
	Mat confidenceData;
	Mat ambientData;
	Mat phaseData;
	bool frameReceived;
};

class CaptureDevice
{
public:
	CaptureDevice();
	VideoCapture cap;
	virtual void release() = 0;
	virtual struct captureData capture() = 0;
};

class VoxelCaptureDevice : public CaptureDevice
{
public:
	int rows, cols;
	VoxelCaptureDevice(int row = 240, int col = 320, int devnum = 0);
	struct captureData capture();
	void release();
};

#endif // CAPTURE_H
