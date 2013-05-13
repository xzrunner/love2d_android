LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libogg

LOCAL_C_INCLUDES  :=  \
	$(LOCAL_PATH)/Include \
	
LOCAL_SRC_FILES := \
	$/src/framing.c \
	$/src/bitwise.c \

LOCAL_LDLIBS :=

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)