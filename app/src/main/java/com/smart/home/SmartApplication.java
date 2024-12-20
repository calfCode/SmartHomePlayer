package com.smart.home;

import android.app.Application;
import android.content.Context;
import android.util.Log;

public class SmartApplication extends Application {

    public static String TAG = SmartApplication.class.getSimpleName();
    public static Context getmInstance() {
        return mInstance;
    }

    private static Context mInstance;

    @Override
    public void onCreate() {
        super.onCreate();
        mInstance = this;
        Log.d(TAG,"onCreate");
    }
}
