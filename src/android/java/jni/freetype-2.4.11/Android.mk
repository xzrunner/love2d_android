LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libfreetype
LOCAL_CFLAGS	:= -DFT2_BUILD_LIBRARY

LOCAL_C_INCLUDES  :=  \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/src \
	
LOCAL_SRC_FILES := \
    $/src/autofit/autofit.c \
    $/src/base/basepic.c \
    $/src/base/ftapi.c \
    $/src/base/ftbase.c \
    $/src/base/ftbbox.c \
    $/src/base/ftbitmap.c \
    $/src/base/ftdbgmem.c \
    $/src/base/ftdebug.c \
    $/src/base/ftglyph.c \
    $/src/base/ftinit.c \
    $/src/base/ftpic.c \
    $/src/base/ftstroke.c \
    $/src/base/ftsynth.c \
    $/src/base/ftsystem.c \
    $/src/cff/cff.c \
    $/src/pshinter/pshinter.c \
    $/src/psnames/psnames.c \
    $/src/raster/raster.c \
    $/src/sfnt/sfnt.c \
    $/src/smooth/smooth.c \
    $/src/truetype/truetype.c 

LOCAL_LDLIBS :=

LOCAL_STATIC_LIBRARIES := 

include $(BUILD_STATIC_LIBRARY)