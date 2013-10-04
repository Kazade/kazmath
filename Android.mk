include $(CLEAR_VARS)
LOCAL_PATH 		:= $(ROOT_PATH)
LOCAL_MODULE	:= kazmath
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/kazmath/*.c)
include $(BUILD_SHARED_LIBRARY) 
