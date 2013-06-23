LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libjpeg

LOCAL_C_INCLUDES  :=  \
	
LOCAL_SRC_FILES := \
	$/jcapimin.c \
	$/jcapistd.c \
	$/jccoefct.c \
	$/jccolor.c \
	$/jcdctmgr.c \
	$/jchuff.c \
	$/jcinit.c \
	$/jcmainct.c \
	$/jcmarker.c \
	$/jcmaster.c \
	$/jcomapi.c \
	$/jcparam.c \
	$/jcphuff.c \
	$/jcprepct.c \
	$/jcsample.c \
	$/jctrans.c \
	$/jdapimin.c \
	$/jdapistd.c \
	$/jdatadst.c \
	$/jdatasrc.c \
	$/jdcoefct.c \
	$/jdcolor.c \
	$/jddctmgr.c \
	$/jdhuff.c \
	$/jdinput.c \
	$/jdmainct.c \
	$/jdmarker.c \
	$/jdmaster.c \
	$/jdmerge.c \
	$/jdphuff.c \
	$/jdpostct.c \
	$/jdsample.c \
	$/jdtrans.c \
	$/jerror.c \
	$/jfdctflt.c \
	$/jfdctfst.c \
	$/jfdctint.c \
	$/jidctflt.c \
	$/jidctred.c \
	$/jquant1.c \
	$/jquant2.c \
	$/jutils.c \
	$/jmemmgr.c \
	$/jmem-android.c \
	
ifeq ($(strip $(ANDROID_JPEG_NO_ASSEMBLER)),true)
LOCAL_SRC_FILES += $/jidctint.c $/jidctfst.c
else
LOCAL_SRC_FILES += $/jidctint.c $/jidctfst.S
endif

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_STATIC_LIBRARY)