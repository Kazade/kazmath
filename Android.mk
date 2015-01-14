LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE	:= kazmath
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*/*.c)
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES:$(LOCAL_PATH)/%=%)
LOCAL_CFLAGS	:= "-std=c99"
include $(BUILD_SHARED_LIBRARY)
