LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := liblua

LOCAL_C_INCLUDES  :=  \
	$/src 
		
LOCAL_SRC_FILES := \
	$/src/loadlib.c \
	$/src/lvm.c \
	$/src/ldo.c \
	$/src/lstate.c \
	$/src/ltablib.c \
	$/src/ltm.c \
	$/src/linit.c \
	$/src/liolib.c \
	$/src/ldblib.c \
	$/src/lcode.c \
	$/src/loslib.c \
	$/src/lgc.c \
	$/src/llex.c \
	$/src/lfunc.c \
	$/src/lparser.c \
	$/src/ldump.c \
	$/src/lundump.c \
	$/src/lmathlib.c \
	$/src/lauxlib.c \
	$/src/lopcodes.c \
	$/src/lapi.c \
	$/src/lmem.c \
	$/src/lstrlib.c \
	$/src/lzio.c \
	$/src/lstring.c \
	$/src/lbaselib.c \
	$/src/ltable.c \
	$/src/ldebug.c \
	$/src/lobject.c \

LOCAL_LDLIBS    := -llog -L../lib -lGLESv1_CM

LOCAL_STATIC_LIBRARIES :=  

include $(BUILD_STATIC_LIBRARY)