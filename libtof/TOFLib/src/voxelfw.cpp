/*******************************************************
 *
 *  Copyright (c) 2014 Texas Instruments Incorporated
 *
 *******************************************************/

/*
 * voxelfw.cpp : Example code to download firmware to the VoxelA
 * This program downloads a firmware image to a USB device with VID:PID hardcoded below
 * This library depends on libusbX - mainly because of the libusb_open_device_with_vid_pid
 * function call. It can also be adapted to work with libusb-1.0
 *
 * Build command to statically link with libusbX:
 *   gcc --static fw_downloader.c -I<path-to-libusbx_install/include> -L<path-to-libusbx_install/lib> -lusb-1.0 -lpthread -ludev -lrt
 *
 */
#include "voxelfw-linux.h"

#define VOXEL_VID 0x0451
#define VOXEL_PID 0x9102

extern "C" void send_file_size(libusb_device_handle *device)
{
	uint8_t buffer[4];
	int status;

	/* FIXME: Explain these magic numbers */
	buffer[0] = 0x89;
	buffer[1] = 0xCB;
	buffer[2] = 0x07;
	buffer[3] = 0x00;

	status = libusb_control_transfer(device,
			LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE,
			(uint8_t) 0x05,
			(uint16_t) 0,
			(uint16_t) 0,
			buffer, 4, 5000);
	if (status != 4) {
        logMessage("Control transfer issue: Status %d\n", status);
	}
}

extern "C" void send_sp_image(libusb_device_handle *device, FILE *fw_file, unsigned long filesize)
{
	unsigned char endpoint_out = 0x06;
	unsigned char buffer[4096];
	int transfer_size;
	unsigned long bytes_to_read = filesize;
	int transferred = 0;
	int status;

	while (bytes_to_read > 0) {
		transfer_size = bytes_to_read > 4096 ? 4096 : bytes_to_read;
		status = fread(buffer, 1, transfer_size, fw_file);
		if (status < transfer_size)
            logMessage("Read less bytes than expected\n");
		libusb_bulk_transfer(device, endpoint_out, buffer,
					transfer_size, &transferred, 10000);
		bytes_to_read -= transfer_size;
	}

}

namespace FW_DOWNLOADER
{

extern "C" void setVerbose(bool _verbose)
{
	verbose = _verbose;
}

extern "C" int sendsp(char *filename)
{
	libusb_device **devs;
	int status;
	libusb_device_handle *device = NULL;
	FILE *fw_file;
	int vid = VOXEL_VID, pid = VOXEL_PID;
	unsigned long filesize;

	status = libusb_init(NULL);
	if (status < 0)
		return status;


	device = libusb_open_device_with_vid_pid(NULL, (uint16_t) vid,
							(uint16_t) pid);
	if (device == NULL) {
        logMessage("Could not open device\n");
		goto err;
	}
    /*
     * This next function is available only with libusbX
     * There's no kernel driver normally for the voxel boards
     * so we can remove this call for now.
     * libusb_set_auto_detach_kernel_driver(device, 1);
    */
	status = libusb_claim_interface(device, 0);
	if (status != LIBUSB_SUCCESS) {
        logMessage("Could not claim the first interface\n");
		goto err;
	}

	fw_file = fopen(filename, "rb");
	if (fw_file) {
        logMessage("Opened FW file successfully\n");
		fseek (fw_file, 0, SEEK_END);
		filesize = ftell(fw_file);
        logMessage("File Size %ld bytes\n", filesize);
		rewind(fw_file);
	} else {
        logMessage("Could not open file %s\n", filename);
		goto err;
	}

	send_file_size(device);
	send_sp_image(device, fw_file, filesize);

	fclose(fw_file);
	libusb_release_interface(device, 0);
	libusb_close(device);
	libusb_exit(NULL);
	return 0;

err:
	libusb_exit(NULL);
	return -1;
}

}
