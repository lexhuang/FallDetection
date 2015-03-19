/*******************************************************
*
*  Copyright (c) 2014 Texas Instruments Incorporated
*
*******************************************************/

#include "capture.h"

CaptureDevice::CaptureDevice()
{
}

VoxelCaptureDevice::VoxelCaptureDevice(int row, int col, int devnum)
{
	cap.open(devnum);
	bool isOpened = cap.isOpened();
	if (isOpened)
		logMessage("Opened camera %d successfully\n", devnum);
	else
		logMessage("Could not open camera %d for capture\n", devnum);

	if (isOpened) {
		cap.set(CV_CAP_PROP_FRAME_WIDTH, col * 2);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, row);
	}
	rows = row;
	cols = col;
}

struct captureData VoxelCaptureDevice::capture()
{
	//long int e1, e2; # Used only for profiling
	// Create buffers
	Mat frame;
	Mat confidence(rows, cols, CV_16S, Scalar(0));
	Mat ambient(rows, cols, CV_8UC1, Scalar(0));
	Mat phase(rows, cols, CV_16S, Scalar(0));
	Mat flags(rows, cols, CV_8UC1, Scalar(0));
	struct captureData info;
	cap >> frame;   // Capture one frame
	// And parse it to extract phase and confidence

	if (frame.rows == rows && frame.cols == cols * 2) {
		//e1 = cv::getTickCount();
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols / 16; j++) {
				for (int k = 0; k < 8; k++) {
					uint16_t conf_data = frame.at<uint16_t>(i, 16 * j + k);
					uint16_t phase_data = frame.at<uint16_t>(i, 16 * j + k + 8);

					confidence.at<uint16_t>(i, 8 * j + k) = (uint16_t)((conf_data & 0x0FFF) << 0);
					phase.at<uint16_t>(i, 8 * j + k) = (uint16_t)((phase_data & 0x0FFF) << 0);

					ambient.at<uint8_t>(i, 8 * j + k) = (uint8_t)((conf_data & 0xF000) >> 4);
					flags.at<uint8_t>(i, 8 * j + k) = (uint8_t)((phase_data & 0xF000) >> 4);
				}
			}
		}
		//e2 = cv::getTickCount();
		//logMessage("%d \n", (e2-e1));
		info.frameReceived = true;
	}
	else {
		logMessage("Could not get frame\n");
		info.frameReceived = false;
	}
	info.frame = frame;
	info.ambientData = ambient;
	info.confidenceData = confidence;
	info.phaseData = phase;
	return info;
}

void VoxelCaptureDevice::release()
{
	cap.release();
}


