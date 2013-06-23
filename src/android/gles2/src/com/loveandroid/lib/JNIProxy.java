package com.loveandroid.lib;

public class JNIProxy {
	
	static {
		System.loadLibrary("ogg");
		System.loadLibrary("love_test");
	}
	
	public static native void nativeSetPaths(String apkPath);
	
	public static native void nativeTouchesBegin(int id, float x, float y);
	public static native void nativeTouchesEnd(int id, float x, float y);
	public static native void nativeTouchesMove(int[] id, float[] x, float[] y);
	public static native void nativeTouchesCancel(int[] id, float[] x, float[] y);
	public static native boolean nativeKeyDown(int keyCode);
	
	public static native void nativeInit(String file);
	public static native void nativeResize(int w, int h);
	public static native void nativeRender();
	
	public static native void nativeOnPause();
	public static native void nativeOnResume();
	
}
