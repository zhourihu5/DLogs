#include <jni.h>
#include "TXFileLogger.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_tencent_logs_FileLogger_nativeCreate(JNIEnv *env, jobject instance, jstring logPath_,
    jlong fileMaxSize) {
    const char *logPth = env->GetStringUTFChars(logPath_, JNI_FALSE);
    TXFileLogger *txFileLogger = new TXFileLogger(logPth, fileMaxSize);
    env->ReleaseStringUTFChars(logPath_, logPth);
    return (jlong) txFileLogger;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tencent_logs_FileLogger_nWrite(JNIEnv *env, jobject thiz, jlong nativePtr,
    jbyteArray data_, jint dataLen) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    TXFileLogger *txFileLogger = reinterpret_cast<TXFileLogger *>(nativePtr);
    txFileLogger->writeData(data, dataLen);
    env->ReleaseByteArrayElements(data_,data, 0);
}