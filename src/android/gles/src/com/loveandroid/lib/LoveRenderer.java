package com.loveandroid.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class LoveRenderer implements GLSurfaceView.Renderer {
	private final static long NANOSECONDSPERSECOND = 1000000000L;
	private final static long NANOSECONDSPERMINISECOND = 1000000;
	private static long animationInterval = (long)(1.0 / 60 * NANOSECONDSPERSECOND);
	private long last;
	
	String filePath;
    
	public LoveRenderer(String filePath) {
		this.filePath = filePath;
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		
		long now = System.nanoTime();
		long interval = now - last;
		
		JNIProxy.nativeRender();
		
		if (interval < animationInterval) {
			try {
				// *2 ??
				Thread.sleep((animationInterval - interval) * 2 / NANOSECONDSPERMINISECOND);
			} catch (Exception e){}
		}
		
		last = now;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		JNIProxy.nativeResize(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		JNIProxy.nativeInit(filePath);
		last = System.nanoTime();
	}
    
    public void handleActionDown(int id, float x, float y) {
    	JNIProxy.nativeTouchesBegin(id, x, y);
    }
    
    public void handleActionUp(int id, float x, float y) {
    	JNIProxy.nativeTouchesEnd(id, x, y);
    }
    
    public void handleActionCancel(int[] id, float[] x, float[] y) {
    	JNIProxy.nativeTouchesCancel(id, x, y);
    }
    
    public void handleActionMove(int[] id, float[] x, float[] y) {
    	JNIProxy.nativeTouchesMove(id, x, y);
    }
    
    public boolean handleKeyDown(int keyCode) {
    	return JNIProxy.nativeKeyDown(keyCode);
    }
    
    public void handleOnPause() {
    	JNIProxy.nativeOnPause();
    }
    
    public void handleOnResume() {
    	JNIProxy.nativeOnResume();
    }
}
