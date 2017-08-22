LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := changemac
LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES := changemac.c

include $(BUILD_EXECUTABLE)
