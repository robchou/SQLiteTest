#include <jni.h>

extern "C" {
    JNIEXPORT jstring JNICALL
        Java_com_androidhive_sqlitetest_DataBaseHelper_stringFromJNI(JNIEnv *env, jobject instance) {
            return env->NewStringUTF("String from JNI");
        }
}
