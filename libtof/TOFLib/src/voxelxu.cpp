#include "voxelxuLinux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include <linux/usb/video.h>

typedef uint8_t BYTE;

static char dev_name[255];
static int fd = -1;
static int selectedCamera = 0;
static bool verbose = false;

static void errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	return;
	//exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
	int r;

	do {
		r = ioctl(fh, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}

static void init_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct uvc_xu_control_query uvc;
	unsigned int min;
	__u8 buf[64];

	bzero(&uvc, sizeof(uvc));
	bzero(buf, sizeof(buf));

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n",
				dev_name);
			return;
		}
		else {
			return;
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
			dev_name);
		return;
	}
	if (verbose)
		logMessage("video capabililty = 0x%08X\n", cap.capabilities);
}

static void open_device(void)
{
	struct stat st;

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n",
			dev_name, errno, strerror(errno));
		return;
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		return;
	}

	fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n",
			dev_name, errno, strerror(errno));
		return;
	}
}

static void close_device(void)
{
	if (-1 == close(fd)) {
		return;
	}
	fd = -1;
}

using namespace std;

namespace Voxelxu
{

	int printTest(void)
	{
		if (verbose)
			logMessage("this is a test!\n");
		return 1;
	}

	void setVerbose(bool __verbose)
	{
		verbose = __verbose;
	}

	int setCur(int controlnumber, int size, uint8_t *value)
	{
		struct uvc_xu_control_query uvc;

		if (verbose)
			logMessage("SetCur: control %d, size %d, value[0] 0x%x\n",
			controlnumber, size, value[0]);
		if (selectedCamera != -1){
			sprintf(dev_name, "/dev/video%d", selectedCamera);
			open_device();
			init_device();
			if (verbose)
				logMessage("Init device done\n");
			uvc.unit = 3;
			uvc.selector = controlnumber;
			uvc.query = UVC_SET_CUR;
			uvc.size = size;
			uvc.data = value;

			if (-1 == xioctl(fd, UVCIOC_CTRL_QUERY, &uvc)) {
				if (EINVAL == errno) {
					fprintf(stderr, "%s UVCIOC_CTRL_QUERY failed.\n",
						dev_name);
					return -1;
				}
				else {
					return -1;
				}
			}
			close_device();
		}
		return 0;
	}

	int getCur(int controlnumber, int size, uint8_t *value)
	{
		struct uvc_xu_control_query uvc;

		if (verbose)
			logMessage("GetCur: control %d, size %d, value[0] 0x%x\n",
			controlnumber, size, value[0]);
		if (selectedCamera != -1){
			sprintf(dev_name, "/dev/video%d", selectedCamera);
			open_device();
			init_device();
			if (verbose)
				logMessage("Init device done\n");
			uvc.unit = 3;
			uvc.selector = controlnumber;
			uvc.query = UVC_GET_CUR;
			uvc.size = size;
			uvc.data = value;
			if (-1 == xioctl(fd, UVCIOC_CTRL_QUERY, &uvc)) {
				if (EINVAL == errno) {
					fprintf(stderr, "%s UVCIOC_CTRL_QUERY failed.\n",
						dev_name);
					return -1;
				}
				else {
					return -1;
				}
			}
			if (verbose)
				logMessage("GetCur: control %d, size %d, value[0] 0x%x\n", controlnumber, size, value[0]);
			close_device();
		}
		return 0;
	}

	void selectDevice(int devnum)
	{
		selectedCamera = devnum;
	}

	/* Not yet implemented. Kept here to maintain compatibility with the Windows version */
	int getDevice(void)
	{
		return 0;
	}

	int numDevices(void)
	{
		return 0;
	}

	void enumerateDevices(void)
	{
	}

}
