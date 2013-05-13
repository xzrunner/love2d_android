LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE 	:= libopenal
LOCAL_CFLAGS	:= -DAL_ALEXT_PROTOTYPES

LOCAL_C_INCLUDES  :=  \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/OpenAL32/Include \
	
LOCAL_SRC_FILES := \
	$/OpenAL32/alAuxEffectSlot.c \
	$/OpenAL32/alBuffer.c \
	$/OpenAL32/alEffect.c \
	$/OpenAL32/alError.c \
	$/OpenAL32/alExtension.c \
	$/OpenAL32/alFilter.c \
	$/OpenAL32/alListener.c \
	$/OpenAL32/alSource.c \
	$/OpenAL32/alState.c \
	$/OpenAL32/alThunk.c \
	$/Alc/ALc.c \
	$/Alc/ALu.c \
	$/Alc/alcConfig.c \
	$/Alc/alcDedicated.c \
	$/Alc/alcEcho.c \
	$/Alc/alcModulator.c \
	$/Alc/alcReverb.c \
	$/Alc/alcRing.c \
	$/Alc/alcThread.c \
	$/Alc/bs2b.c \
	$/Alc/helpers.c \
	$/Alc/hrtf.c \
	$/Alc/mixer.c \
	$/Alc/panning.c \
	$/Alc/backends/loopback.c \
	$/Alc/backends/null.c \
	$/Alc/backends/android.c \
	$/Alc/backends/wave.c \
	$/Alc/backends/opensl.c

LOCAL_LDLIBS := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_STATIC_LIBRARY)