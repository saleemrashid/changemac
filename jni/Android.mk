LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -Wall -Wextra -Werror

LOCAL_MODULE := changemac
LOCAL_LDLIBS := -llog

ifneq ($(WCNSS_DEVICE_ADDRESS),)
LOCAL_CFLAGS += -DWCNSS_DEVICE_ADDRESS="\"$(WCNSS_DEVICE_ADDRESS)\""
endif

LOCAL_SRC_FILES := changemac.c

include $(BUILD_EXECUTABLE)
