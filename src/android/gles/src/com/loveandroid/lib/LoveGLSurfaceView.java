package com.loveandroid.lib;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class LoveGLSurfaceView extends GLSurfaceView {

	private LoveRenderer mRenderer;
	
	public LoveGLSurfaceView(Context context, String filePath) {
		super(context);
		
		mRenderer = new LoveRenderer(filePath);
		setRenderer(mRenderer);
		
		setFocusableInTouchMode(true);
	}
    
    public void onPause(){
    	queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.handleOnPause();
            }
        });
    	
    	super.onPause();
    }
    
    public void onResume(){
    	super.onResume();
    	
    	queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.handleOnResume();
            }
        });
    }
    
    public boolean onTouchEvent(final MotionEvent event) {
    	// these data are used in ACTION_MOVE and ACTION_CANCEL
    	final int pointerNumber = event.getPointerCount();
    	final int[] ids = new int[pointerNumber];
    	final float[] xs = new float[pointerNumber];
    	final float[] ys = new float[pointerNumber];

    	for (int i = 0; i < pointerNumber; i++) {
            ids[i] = event.getPointerId(i);
            xs[i] = event.getX(i);
            ys[i] = event.getY(i);
        }
        
        switch (event.getAction() & MotionEvent.ACTION_MASK) {
        case MotionEvent.ACTION_POINTER_DOWN:
        	final int indexPointerDown = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
        	final int idPointerDown = event.getPointerId(indexPointerDown);
            final float xPointerDown = event.getX(indexPointerDown);
            final float yPointerDown = event.getY(indexPointerDown);

            queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.handleActionDown(idPointerDown, xPointerDown, yPointerDown);
                }
            });
            break;
            
        case MotionEvent.ACTION_DOWN:
        	// there are only one finger on the screen
        	final int idDown = event.getPointerId(0);
            final float xDown = xs[0];
            final float yDown = ys[0];
            
            queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.handleActionDown(idDown, xDown, yDown);
                }
            });
            break;

        case MotionEvent.ACTION_MOVE:
            queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.handleActionMove(ids, xs, ys);
                }
            });
            break;

        case MotionEvent.ACTION_POINTER_UP:
        	final int indexPointUp = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
        	final int idPointerUp = event.getPointerId(indexPointUp);
            final float xPointerUp = event.getX(indexPointUp);
            final float yPointerUp = event.getY(indexPointUp);
            
            queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.handleActionUp(idPointerUp, xPointerUp, yPointerUp);
                }
            });
            break;
            
        case MotionEvent.ACTION_UP:  
        	// there are only one finger on the screen
        	final int idUp = event.getPointerId(0);
            final float xUp = xs[0];
            final float yUp = ys[0];
            
            queueEvent(new Runnable() {
                @Override
                public void run() {
                    mRenderer.handleActionUp(idUp, xUp, yUp);
                }
            });
            break;

        case MotionEvent.ACTION_CANCEL:
            queueEvent(new Runnable() {
               @Override
                public void run() {
                    mRenderer.handleActionCancel(ids, xs, ys);
                }
            });
            break;
        }
      
        return true;
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
//    	final int kc = keyCode;
//    	if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_MENU) {
//    		queueEvent(new Runnable() {
//	            @Override
//	            public void run() {
//	                mRenderer.handleKeyDown(kc);
//	            }
//    		});
//    		return true;
//    	}
        return super.onKeyDown(keyCode, event);
    }
    
    protected void onSizeChanged(int w, int h, int oldw, int oldh){
//    	this.mRenderer.setScreenWidthAndHeight(w, h);
    }
}
