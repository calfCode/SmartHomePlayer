package com.smart.home;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements View.OnClickListener {

    public static String TAG = "MainActivity";

    private RaopServer mRaopServer;


    private SurfaceView mSurfaceView;
    private Button mBtnControl;
    private TextView mTxtDevice;
    private boolean mIsStart = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getSystemService(Context.NSD_SERVICE);
        mBtnControl = findViewById(R.id.btn_control);
        mTxtDevice = findViewById(R.id.txt_device);
        mBtnControl.setOnClickListener(this);
        mSurfaceView = findViewById(R.id.surface);
        mRaopServer = new RaopServer(mSurfaceView);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.btn_control) {
            if (!mIsStart) {
                startServer();
                int raopPort = mRaopServer.getPort();
                mTxtDevice.setText("raopPort=" + raopPort);
            } else {
                stopServer();
                mTxtDevice.setText("未启动");
            }
            mIsStart = !mIsStart;
            mBtnControl.setText(mIsStart ? "结束" : "开始");
        }
    }

    private void startServer() {
        mRaopServer.startServer();
    }

    private void stopServer() {

        mRaopServer.stopServer();
    }




}


