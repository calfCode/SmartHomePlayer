package com.smart.home;


import android.content.Intent;
import android.util.Log;

public class AirplayLogic {
    public static String TAG = "AirplayLogic";
    public static void airplayPlayUrl(String url,float postion){
        Log.d(TAG,"airplayPlayUrl url="+url+";postion="+postion);
        Intent intent = new Intent();
        intent.setClassName(SmartApplication.getmInstance(),PlayActivity.class.getName());
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.putExtra("url", url);
        intent.putExtra("name", "AirPlay");
        intent.putExtra("pos", (double) postion);
        intent.putExtra("isLooping", false);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
        SmartApplication.getmInstance().startActivity(intent);
    }

    public static void tearDown(){
        Log.d(TAG,"tearDown");
//        Intent intent = new Intent(VIDEO_STOP);
//        DeApplication.getmInstance().sendBroadcast(intent);
    }
}
