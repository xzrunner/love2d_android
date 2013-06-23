LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libvorbis

LOCAL_C_INCLUDES  :=  \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/../libogg-1.3.0/include \
	
LOCAL_SRC_FILES := \
	$/lib/vorbisfile.c \
	$/lib/mdct.c \
	$/lib/smallft.c \
	$/lib/block.c \
	$/lib/envelope.c \
	$/lib/window.c \
	$/lib/lsp.c \
	$/lib/lpc.c \
	$/lib/analysis.c \
	$/lib/synthesis.c \
	$/lib/psy.c \
	$/lib/info.c \
	$/lib/floor1.c \
	$/lib/floor0.c \
	$/lib/res0.c \
	$/lib/mapping0.c \
	$/lib/registry.c \
	$/lib/codebook.c \
	$/lib/sharedbook.c \
	$/lib/lookup.c \
	$/lib/bitrate.c \

LOCAL_LDLIBS :=

LOCAL_SHARED_LIBRARY := libogg

#$(call import-add-path,$(LOCAL_PATH)/..)
#$(call import-module, libogg-1.3.0)

include $(BUILD_STATIC_LIBRARY)