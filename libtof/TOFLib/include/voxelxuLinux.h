#pragma once
/*******************************************************
*
*  Copyright (c) 2014 Texas Instruments Incorporated
*
*******************************************************/

// voxelxu.h

//#define VOXELXU_EXPORTS

#include "Globals.h"
#ifdef VOXELXU_EXPORTS
#define VOXELXU_API __declspec(dllexport) 
#else
//#define VOXELXU_API __declspec(dllimport) 
#define VOXELXU_API
#endif

#define MAX_CAMERAS 12

//DEFINE_GUIDSTRUCT("8A0F88DD-BA1C-5449-8A25-F7875967F0F7", PROPSETID_FX2_XU);
//#define PROPSETID_FX2_XU DEFINE_GUIDNAMED(PROPSETID_FX2_XU)

#include <stdint.h>
typedef uint8_t BYTE;

namespace Voxelxu
{
	VOXELXU_API int printTest(void);

	VOXELXU_API void setVerbose(bool __verbose);

	VOXELXU_API int setCur(int controlnumber, int size, BYTE *value);

	VOXELXU_API int getCur(int controlnumber, int size, BYTE *value);

	VOXELXU_API void selectDevice(int devnum);

	VOXELXU_API int getDevice(void);

	VOXELXU_API char * getDeviceName(int devnum);

	VOXELXU_API int numDevices(void);

	VOXELXU_API void enumerateDevices(void);
}
