LOCAL_PATH 			:= $(call my-dir)
PATH_TO_TOF			:= /home/lexhuang/workspace/libtof/TOFLib
PATH_TO_USB			:= /home/lexhuang/workspace/libusb

include $(CLEAR_VARS)  

LOCAL_MODULE    := tof  
LOCAL_SRC_FILES := libtof.so

include $(PREBUILT_SHARED_LIBRARY)  
  
include $(CLEAR_VARS)  

LOCAL_MODULE    := usb1.0
LOCAL_SRC_FILES := libusb1.0.so

include $(PREBUILT_SHARED_LIBRARY)  


include $(CLEAR_VARS)

LOCAL_MODULE    	:= FallDetection
LOCAL_SRC_FILES 	:= FallDetection.cpp
LOCAL_LDLIBS    	+= -llog
LOCAL_C_INCLUDES 	+= $(PATH_TO_USB)
LOCAL_C_INCLUDES	+= $(PATH_TO_TOF)/include
LOCAL_SHARED_LIBRARIES := tof

include $(BUILD_SHARED_LIBRARY)