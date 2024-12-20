package com.smart.home;

import static androidx.media3.common.Player.REPEAT_MODE_ALL;
import static androidx.media3.common.Player.REPEAT_MODE_OFF;


import static com.smart.home.MirrorConstant.VIDEO_STOP;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;

import androidx.media3.common.MediaItem;
import androidx.media3.common.PlaybackException;
import androidx.media3.common.Player;
import androidx.media3.exoplayer.ExoPlayer;
import androidx.media3.ui.PlayerView;


/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class PlayActivity extends Activity {
    private PlayerView mPlayerView;
    private ExoPlayer mExoPlayer;
    String TAG = PlayActivity.class.getSimpleName();

    private String lastUrl = null;
    private String name;
    private double pos = 0;

    private MyReceiver recevier;
    private IntentFilter intentFilter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        mPlayerView = findViewById(R.id.player_view);
        recevier = new MyReceiver();
        intentFilter = new IntentFilter();
        intentFilter.addAction(VIDEO_STOP);
        registerReceiver(recevier, intentFilter);
    }

    @Override
    protected void onResume() {
        Log.i(TAG,"onResume");
        super.onResume();
        if (mExoPlayer==null){
            initializePlayer();
        }
    }

    protected void initializePlayer() {
        if (mExoPlayer == null) {
            mExoPlayer = new ExoPlayer.Builder(this).build();
            mPlayerView.setPlayer(mExoPlayer);
        }
        Log.d(TAG,"initializePlayer ");
        Intent intent = getIntent();
        startPlay(intent);
    }

    private void startPlay(Intent intent){
        String url = intent.getStringExtra("url");
        Log.d(TAG,"startPlay url="+ url);
        boolean isLooping=   intent.getBooleanExtra("isLooping",false);
        Log.d(TAG,"startPlay isLooping="+ isLooping);
//        if (!TextUtils.isEmpty(lastUrl) && lastUrl.equals(url)) {
//            Log.d(TAG, "run: This Url same to last Url");
//            return;
//        }

        lastUrl = url;

        name = intent.getStringExtra("name");
        pos = intent.getDoubleExtra("pos", 0);
        Log.d(TAG,"startPlay pos="+ pos);
        mExoPlayer.addMediaItem(MediaItem.fromUri(url));
        if (isLooping){
            mExoPlayer.setRepeatMode(REPEAT_MODE_ALL);
        }else {
            mExoPlayer.setRepeatMode(REPEAT_MODE_OFF);
        }
//        HlsMediaSource hlsMediaSource =
//                new HlsMediaSource.Factory(dataSourceFactory)
//                        .setAllowChunklessPreparation(false)
//                        .createMediaSource(MediaItem.fromUri(url));
//        mExoPlayer.setMediaSource(hlsMediaSource);
        if (pos>1) {
            mExoPlayer.seekTo((long) pos);
        }
        mExoPlayer.prepare();
        mExoPlayer.addListener(new Player.Listener() {

            @Override
            public void onPlaybackStateChanged(int playbackState) {
                Player.Listener.super.onPlaybackStateChanged(playbackState);
                if (playbackState == Player.STATE_ENDED){
                    Log.i(TAG,"onPlaybackStateChanged STATE_ENDED");
                    if(mExoPlayer.getRepeatMode()== REPEAT_MODE_OFF){
                        Log.i(TAG,"onPlaybackStateChanged finish");
//                        finish();
                    }
                }
            }


            @Override
            public void onPlayerError(PlaybackException error) {
                Player.Listener.super.onPlayerError(error);
                Log.i(TAG,"onPlayerError finish");
//                finish();
            }
        });
        mExoPlayer.setPlayWhenReady(true);
    }

    public class MyReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.i(TAG, "onReceive action=" + action);
            if (VIDEO_STOP.equals(action)) {
                stop();
            }
        }
    }


    public void stop() {
        finish();
    }

    protected void releasePlayer() {
        if (mExoPlayer != null) {
            mExoPlayer.release();
            mExoPlayer = null;
            mPlayerView.setPlayer(/* player= */ null);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (recevier!=null){
            unregisterReceiver(recevier);
            recevier = null;
        }
        releasePlayer();
    }
}