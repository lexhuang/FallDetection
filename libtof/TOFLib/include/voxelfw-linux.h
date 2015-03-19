/*******************************************************
 *
 *  Copyright (c) 2014 Texas Instruments Incorporated
 *
 *******************************************************/

#include <stdio.h>
#include "Globals.h"
#include <libusb-1.0/libusb.h>

extern "C" void send_file_size(libusb_device_handle *device);
extern "C" void send_sp_image(libusb_device_handle *device, FILE *fw_file, unsigned long filesize);

static bool verbose = false;

using namespace std;

namespace FW_DOWNLOADER
{

extern "C" void setVerbose(bool _verbose);
extern "C" int sendsp(char *filename);

}
