#include <jni.h>
#include "Log.h"
/* Header for class com_androidhive_sqlitetest_DataBaseHelper */

#ifndef _Included_com_androidhive_sqlitetest_DataBaseHelper
#define _Included_com_androidhive_sqlitetest_DataBaseHelper
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
    /*
     * Class:     com_androidhive_sqlitetest_DataBaseHelper
     * Method:    stringFromJNI
     * Signature: ()Ljava/lang/String;
     */
    JNIEXPORT jstring JNICALL 
        Java_com_androidhive_sqlitetest_DataBaseHelper_stringFromJNI (JNIEnv *env, jobject obj) {

            return env->NewStringUTF("String from JNI");
        }

    /*
     * Class:     com_androidhive_sqlitetest_DataBaseHelper
     * Method:    writeStringToFile
     * Signature: (Ljava/lang/String;Ljava/lang/String;)V
     */
    JNIEXPORT void JNICALL 
        Java_com_androidhive_sqlitetest_DataBaseHelper_writeStringToFile
        (JNIEnv *env, jobject obj, jstring path, jstring content) {
            FILE *fp;
            const char* pathNativeString = env->GetStringUTFChars(path, 0);
            const char* contentNativeString = env->GetStringUTFChars(content, 0);
            if ((fp = fopen(pathNativeString, "w+")) == NULL) {
                LOGE("open failed\n");
                return;
            } else {
                fputs(contentNativeString, fp);
            }

            fclose(fp);
            env->ReleaseStringUTFChars(path, pathNativeString);
            env->ReleaseStringUTFChars(content, contentNativeString);
            return;
        }

#ifdef __cplusplus
}
#endif
#endif
