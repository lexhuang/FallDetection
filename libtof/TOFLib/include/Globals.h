#pragma once
/*******************************************************
*
*  Copyright (c) 2014 Texas Instruments Incorporated
*
*******************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdarg.h>
#include <stdio.h>

#include <stdint.h>

#include <unistd.h>

#define PRINT_LOGS false    //Indicates whether logMessage() prints are enabled or not

void sleepFunction(int milliSeconds);

void logMessage(const char *message, ...);

#endif // GLOBALS_H
