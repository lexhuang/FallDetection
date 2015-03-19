#include "Globals.h"

/*
* Override default print behavior with logMessage()
* Prints through this function are enabled only when
* PRINT_LOGS is true. This variable id defined in Globals.h
*/
void logMessage(const char *message, ...) {
	if (PRINT_LOGS == true) {
		va_list args;
		va_start(args, message);
		vfprintf(stdout, message, args);
		va_end(args);
	}
}

/*
* This function sleeps for "milliSeconds" ms
* It is generic since different functions exist
* for sleep in linux and windows
*/
void sleepFunction(int milliSeconds) {
	float sleepTime = milliSeconds / 1000.0;
	sleep(sleepTime);
}
