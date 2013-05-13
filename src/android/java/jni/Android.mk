#LOCAL_PATH := $(call my-dir)

LOCAL_PATH := $(call my-dir)
MY_PATH := $(LOCAL_PATH)
#include $(call all-makefiles-under,$(LOCAL_PATH))

include $(MY_PATH)/liblua/Android.mk
include $(MY_PATH)/libpng/Android.mk
include $(MY_PATH)/libjpeg/Android.mk
include $(MY_PATH)/box2d/Android.mk
include $(MY_PATH)/openal-soft-android/Android.mk
include $(MY_PATH)/libmodplug-0.8.8.4/Android.mk
include $(MY_PATH)/freetype-2.4.11/Android.mk
include $(MY_PATH)/libogg-1.3.0/Android.mk
include $(MY_PATH)/libvorbis-1.3.3/Android.mk

LOCAL_PATH := $(MY_PATH)

LUA_SRC_PATH		:= $(LOCAL_PATH)/liblua/
PNG_SRC_PATH 		:= $(LOCAL_PATH)/libpng/
JPEG_SRC_PATH 		:= $(LOCAL_PATH)/libjpeg/
BOX2D_SRC_PATH		:= $(LOCAL_PATH)/box2d/
OPENAL_SRC_PATH		:= $(LOCAL_PATH)/openal-soft-android/
MODPLUG_SRC_PATH	:= $(LOCAL_PATH)/libmodplug-0.8.8.4/
OGG_SRC_PATH		:= $(LOCAL_PATH)/libogg-1.3.0/
VORBIS_SRC_PATH		:= $(LOCAL_PATH)/libvorbis-1.3.3/
FREETYPE_SRC_PATH	:= $(LOCAL_PATH)/freetype-2.4.11/
LOVE_SRC_PATH		:= ../../../love2d_android/
ZIP_SRC_PATH 		:= ../../../love2d_android/libraries/Minizip/

# liblove
include $(CLEAR_VARS)

LOCAL_MODULE	:= liblove
LOCAL_CFLAGS    := -DRAPIDXML_NO_EXCEPTIONS -DLOVE_NOMPG123

LOCAL_C_INCLUDES := \
	${VORBIS_SRC_PATH}include \
	${OGG_SRC_PATH}include \
	${LUA_SRC_PATH}src \
	${OPENAL_SRC_PATH}include \
	${MODPLUG_SRC_PATH}src \
	${FREETYPE_SRC_PATH}include \
	${PNG_SRC_PATH} \
	${BOX2D_SRC_PATH} \
	$(LOCAL_PATH)/../../../love2d_android \
	$(LOCAL_PATH)/../../../love2d_android/modules \
	$(LOCAL_PATH)/../../../love2d_android/libraries \
	
LOCAL_SRC_FILES := \
	${ZIP_SRC_PATH}unzip.c \
	${ZIP_SRC_PATH}ioapi.c \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}common -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/audio -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/event -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/filesystem -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/font -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/graphics -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/image -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/keyboard -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/mouse -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/physics -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/sound -name "*.cpp" -print)) \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/${LOVE_SRC_PATH}modules/timer -name "*.cpp" -print)) \
	${LOVE_SRC_PATH}modules/thread/posix/threads.cpp \
	${LOVE_SRC_PATH}love_android.cpp \

LOCAL_LDLIBS    := -llog -lEGL -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES	:= \
	liblua \
	box2d_static \
	libjpeg \
	png \
	libopenal \
	libfreetype \
	libmodplug \
	libogg \
	libvorbis \

include $(BUILD_STATIC_LIBRARY)

# love_test
include $(CLEAR_VARS)

LOCAL_MODULE	:= love_test
LOCAL_CFLAGS    := -DRAPIDXML_NO_EXCEPTIONS

LOCAL_C_INCLUDES := \
	${LUA_SRC_PATH}src \
	$(LOCAL_PATH)/../../.. \
	$(LOCAL_PATH)/../../../love2d_android \
	$(LOCAL_PATH)/../../../love2d_android/modules \
	${LOVE_SRC_PATH}src/common/runtime.cpp \

LOCAL_SRC_FILES	:= \
	com_loveandroid_lib_JNIProxy.cpp \

LOCAL_LDLIBS	:= -llog -lEGL -lGLESv1_CM -lOpenSLES

LOCAL_STATIC_LIBRARIES	:= \
	liblove
	
include $(BUILD_SHARED_LIBRARY)