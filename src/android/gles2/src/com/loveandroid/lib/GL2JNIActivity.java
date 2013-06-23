/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.loveandroid.lib;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

import java.io.File;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
    
    private static String packageName;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new GL2JNIView(getApplication());
        
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
		        
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    public static void terminateProcess(){
    	android.os.Process.killProcess(android.os.Process.myPid());
    }
    
    protected void setPackageName(String packageName) {
    	GL2JNIActivity.packageName = packageName;
    	
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
