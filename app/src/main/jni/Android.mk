LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc .cpp

define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALL_FILES := $(call walk, $(LOCAL_PATH)/database)
ALL_FILES += $(wildcard $(LOCAL_PATH)/*.cpp)

FILE_LIST := $(filter %.cpp, $(ALL_FILES))
FILE_LIST += $(filter %.cc, $(ALL_FILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := wxsqlite3

LOCAL_MODULE := im

LOCAL_LDLIBS := -llog -landroid

LOCAL_C_INCLUDES := $(LOCAL_PATH)/thirdpart

LOCAL_CFLAGS := -frtti -fexceptions

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under, $(LOCAL_PATH)/thirdpart)
