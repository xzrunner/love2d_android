#include "com_loveandroid_lib_JNIProxy.h"

#include <love2d_android/modules/filesystem/null/Filesystem.h>
#include <love2d_android/modules/event/eventdefs.h>

extern "C"
{
	extern void init(int argc, char ** argv);
	extern void resize(int w, int h);
	extern void step();
	extern void touch(int type, int x, int y);
}

void Java_com_loveandroid_lib_JNIProxy_nativeSetPaths(JNIEnv* env, jclass, jstring apkPath)
{
	const char* str = env->GetStringUTFChars(apkPath, NULL);
	love::filesystem::null::Filesystem::Instance()->setResourcePath(str);
	env->ReleaseStringUTFChars(apkPath, str);
}

void Java_com_loveandroid_lib_JNIProxy_nativeTouchesBegin(JNIEnv *, jclass, jint, jfloat x, jfloat y)
{
	touch(love::event::LOVE_MOUSEBUTTONDOWN, x, y);
}

void Java_com_loveandroid_lib_JNIProxy_nativeTouchesEnd(JNIEnv *, jclass, jint, jfloat x, jfloat y)
{
	touch(love::event::LOVE_MOUSEBUTTONUP, x, y);
}

void Java_com_loveandroid_lib_JNIProxy_nativeTouchesMove(JNIEnv* env, jclass, jintArray ids, jfloatArray xs, jfloatArray ys)
{
    int size = env->GetArrayLength(ids);
    jint id[size];
    jfloat x[size];
    jfloat y[size];

    env->GetIntArrayRegion(ids, 0, size, id);
    env->GetFloatArrayRegion(xs, 0, size, x);
    env->GetFloatArrayRegion(ys, 0, size, y);

    for (size_t i = 0; i < size; ++i)
    	touch(love::event::LOVE_MOUSEMOTION, x[i], y[i]);
}

void Java_com_loveandroid_lib_JNIProxy_nativeTouchesCancel(JNIEnv *, jclass, jintArray, jfloatArray, jfloatArray)
{
}

jboolean Java_com_loveandroid_lib_JNIProxy_nativeKeyDown(JNIEnv *, jclass, jint keyCode)
{
    return JNI_FALSE;
}

// https://github.com/hagish/love-native-android
void Java_com_loveandroid_lib_JNIProxy_nativeInit(JNIEnv * env, jclass, jstring file)
{
	int argc = 2;
	char **argv = new char *[2];
	argv[0] = new char[5];
	strcpy(argv[0], "love");
	const char * loveFile = env->GetStringUTFChars(file, NULL);
	argv[1] = new char[strlen(loveFile)+1];
	strcpy(argv[1], loveFile);
	env->ReleaseStringUTFChars(file, loveFile);

	init(argc, argv);

	delete argv;
}

void Java_com_loveandroid_lib_JNIProxy_nativeResize(JNIEnv *, jclass, jint w, jint h)
{
	resize(w, h);
}

void Java_com_loveandroid_lib_JNIProxy_nativeRender(JNIEnv *, jclass)
{
	step();
}

void Java_com_loveandroid_lib_JNIProxy_nativeOnPause(JNIEnv *, jclass)
{

}

void Java_com_loveandroid_lib_JNIProxy_nativeOnResume(JNIEnv *, jclass)
{

}
