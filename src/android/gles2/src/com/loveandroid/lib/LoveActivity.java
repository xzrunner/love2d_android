package com.loveandroid.lib;

import java.io.File;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;

public class LoveActivity extends Activity {
	
	private GLSurfaceView mGLView;
	
	private static String packageName;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// get the packageName,it's used to set the resource path
		String packageName = getApplication().getPackageName();
		setPackageName(packageName);
		
		//
		String filePath = "";
		
		Uri data = getIntent().getData();
        if (data != null) {
        	String path = data.getEncodedPath();
        	File file = new File(path);
        	if (file.exists()) {
        		filePath = path;
        	}
        }
		//
        
        Log.d("zz+++++++++++++++", "packageName: " + packageName + ", filePath: " + filePath);
		
		mGLView = new LoveGLSurfaceView(this, filePath);
		setContentView(mGLView);
	}
	
    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
    
    public static void terminateProcess(){
    	android.os.Process.killProcess(android.os.Process.myPid());
    }
    
    protected void setPackageName(String packageName) {
    	LoveActivity.packageName = packageName;
    	
    	String apkFilePath = "";
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = getApplication().getPackageManager();
        try {
            appInfo = packMgmr.getApplicationInfo(packageName, 0);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            throw new RuntimeException("Unable to locate assets, aborting...");
        }
        
        apkFilePath = appInfo.sourceDir;
        Log.w("apk path", apkFilePath);

        JNIProxy.nativeSetPaths(apkFilePath);
    }
}
