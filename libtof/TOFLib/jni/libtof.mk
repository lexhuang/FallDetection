USER_LOCAL_PATH:=$(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

PATH_TO_LIBUSB_SRC	:= /home/lexhuang/workspace/libusb

include $(CLEAR_VARS)

LIBTOF_HEAD_ABS		:= ../include
LIBTOF_SRC_ABS		:= ../src

LOCAL_MODULE 		:= libtof
LOCAL_C_INCLUDES	+= $(LIBTOF_HEAD_ABS)
LOCAL_C_INCLUDES 	+= $(PATH_TO_LIBUSB_SRC)
LOCAL_EXPORT_C_INCLUDES += $(LIBTOF_HEAD_ABS)
LOCAL_EXPORT_C_INCLUDES	+= $(PATH_TO_LIBUSB_SRC)
LOCAL_SRC_FILES += \
		$(LIBTOF_SRC_ABS)/cnumpy.cpp \
		$(LIBTOF_SRC_ABS)/Device.cpp \
		$(LIBTOF_SRC_ABS)/Globals.cpp \
		$(LIBTOF_SRC_ABS)/regProg.cpp \
		$(LIBTOF_SRC_ABS)/tofmanager.cpp \
		$(LIBTOF_SRC_ABS)/voxelfw.cpp \
		$(LIBTOF_SRC_ABS)/voxelxu.cpp
LOCAL_SHARED_LIBRARIES += libusb1.0

LOCAL_CPPFLAGS += -fno-rtti

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH:=$(USER_LOCAL_PATH)

include $(PATH_TO_LIBUSB_SRC)/android/jni/libusb.mk
