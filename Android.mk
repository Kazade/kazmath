include $(CLEAR_VARS)
LOCAL_PATH 		:= $(ROOT_PATH)/lib/kazmath
LOCAL_MODULE	:= kazmath
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/kazmath/*.c))
LOCAL_CFLAGS	:= "-std=c99"
include $(BUILD_SHARED_LIBRARY) 
