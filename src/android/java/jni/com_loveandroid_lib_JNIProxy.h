/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_loveandroid_lib_JNIProxy */

#ifndef _Included_com_loveandroid_lib_JNIProxy
#define _Included_com_loveandroid_lib_JNIProxy
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeSetPaths
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeSetPaths
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeTouchesBegin
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeTouchesBegin
  (JNIEnv *, jclass, jint, jfloat, jfloat);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeTouchesEnd
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeTouchesEnd
  (JNIEnv *, jclass, jint, jfloat, jfloat);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeTouchesMove
 * Signature: ([I[F[F)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeTouchesMove
  (JNIEnv *, jclass, jintArray, jfloatArray, jfloatArray);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeTouchesCancel
 * Signature: ([I[F[F)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeTouchesCancel
  (JNIEnv *, jclass, jintArray, jfloatArray, jfloatArray);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeKeyDown
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_loveandroid_lib_JNIProxy_nativeKeyDown
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeInit
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeInit
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeResize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeResize
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeRender
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeRender
  (JNIEnv *, jclass);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeOnPause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeOnPause
  (JNIEnv *, jclass);

/*
 * Class:     com_loveandroid_lib_JNIProxy
 * Method:    nativeOnResume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_loveandroid_lib_JNIProxy_nativeOnResume
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
