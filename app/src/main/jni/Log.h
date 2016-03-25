//
// Created by ymypl on 2016/2/23.
//

#ifndef IM_LOG_H
#define IM_LOG_H

#ifdef __ANDROID__

#include <android/log.h>

#ifndef MY_LOG_TAG
#define MY_LOG_TAG "Duomai_IM"
#endif

#ifndef MY_LOG_LEVEL
#define MY_LOG_LEVEL ANDROID_LOG_VERBOSE
#endif

#define MY_LOG_PRINT(level, fmt, ...) \
    __android_log_print(level, MY_LOG_TAG, "(%s:%u)%s: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define MY_LOG_NOOP (void)0

#if ANDROID_LOG_VERBOSE >= MY_LOG_LEVEL
#define LOGV(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
        #define LOGV(...) MY_LOG_NOOP
#endif

#if ANDROID_LOG_DEBUG >= MY_LOG_LEVEL
#define LOGD(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
        #define LOGD(...) MY_LOG_NOOP
#endif

#if ANDROID_LOG_INFO >= MY_LOG_LEVEL
#define LOGI(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
        #define LOGI(...) MY_LOG_NOOP
#endif

#if ANDROID_LOG_WARN >= MY_LOG_LEVEL
#define LOGW(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)
#else
        #define LOGW(...) MY_LOG_NOOP
#endif

#if ANDROID_LOG_ERROR >= MY_LOG_LEVEL
#define LOGE(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
        #define LOGE(...) MY_LOG_NOOP
#endif

#if ANDROID_LOG_FATAL >= MY_LOG_LEVEL
#define LOGF(fmt, ...) MY_LOG_PRINT(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
        #define LOGF(...) MY_LOG_NOOP
#endif

#endif //__ANDROID__

#endif //IM_LOG_H
