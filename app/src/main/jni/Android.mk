LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_SRC_FILES += IM.cpp

LOCAL_STATIC_LIBRARIES := wxsqlite3

LOCAL_MODULE := im

LOCAL_LDLIBS := -llog

LOCAL_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under, $(LOCAL_PATH))
