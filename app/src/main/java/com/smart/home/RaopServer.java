package com.smart.home;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.smart.home.model.NALPacket;
import com.smart.home.model.PCMPacket;
import com.smart.home.player.AudioPlayer;
import com.smart.home.player.VideoPlayer;


public class RaopServer implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("aircast");
        System.loadLibrary("play-lib");
    }
    private static final String TAG = "RaopServer";
    private VideoPlayer mVideoPlayer;
    private AudioPlayer mAudioPlayer;
    private SurfaceView mSurfaceView;
    private long mServerId = 0;

    public RaopServer(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        mSurfaceView.getHolder().addCallback(this);
        mAudioPlayer = new AudioPlayer();
        mAudioPlayer.start();
    }

    public void onRecvVideoData(byte[] nal, int nalType, long dts, long pts) {
        Log.d(TAG, "onRecvVideoData pts = " + pts + ", nalType = " + nalType + ", nal length = " + nal.length);
        NALPacket nalPacket = new NALPacket();
        nalPacket.nalData = nal;
        nalPacket.nalType = nalType;
        nalPacket.pts = pts;
        mVideoPlayer.addPacker(nalPacket);
    }

    public void onRecvAudioData(short[] pcm, long pts) {
        Log.d(TAG, "onRecvAudioData pcm length = " + pcm.length + ", pts = " + pts);
        PCMPacket pcmPacket = new PCMPacket();
        pcmPacket.data = pcm;
        pcmPacket.pts = pts;
        mAudioPlayer.addPacker(pcmPacket);
    }

    public void playHLS(String url,float position){

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (mVideoPlayer == null) {
            mVideoPlayer = new VideoPlayer(holder.getSurface());
            mVideoPlayer.start();
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void startServer() {
        if (mServerId == 0) {
            mServerId = start();
        }
    }

    public void stopServer() {
        if (mServerId != 0) {
            stop(mServerId);
        }
        mServerId = 0;

    }

    public int getPort() {
        if (mServerId != 0) {
            return getPort(mServerId);
        }
        return 0;
    }

    private native long start();
    private native void stop(long serverId);
    private native int getPort(long serverId);
    private native long start_raop_server();
}
