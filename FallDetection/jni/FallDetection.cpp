#include <jni.h>

#include <android/log.h>
#include "tofmanager.h"


#define  LOG_TAG    "NDK Log"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace std;

extern "C" {

JNIEXPORT void JNICALL Java_com_ntut_tof_falldetection_TOFManager_startDevice
  (JNIEnv *, jclass){
	LOGD("test");
	TOFManager* manager = new TOFManager();
}

}
