//
// Created by Administrator on 2019/1/29/029.
//

#include <jni.h>
#include <stddef.h>
#include "lib/raop.h"
#include "log.h"
#include "lib/stream.h"
#include "lib/logger.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "j4a_base.h"
#include "jni_helper.h"
#include "airplay_logic.h"
#ifdef __cplusplus
}
#endif
#include <malloc.h>
#include <cstring>



static JavaVM* g_JavaVM;
static unsigned int open_connections= 0;
static dnssd_t *dnssd = NULL;

#define DEFAULT_NAME "UxPlay"

void OnRecvAudioData(void *observer, audio_decode_struct *data) {
    jobject obj = (jobject) observer;
    JNIEnv* jniEnv = NULL;
    g_JavaVM->AttachCurrentThread(&jniEnv, NULL);
    jclass cls = jniEnv->GetObjectClass(obj);
    jmethodID onRecvVideoDataM = jniEnv->GetMethodID(cls, "onRecvAudioData", "([SJ)V");
    jniEnv->DeleteLocalRef(cls);
    jshortArray sarr = jniEnv->NewShortArray(data->data_len);
    if (sarr == NULL) return;
    jniEnv->SetShortArrayRegion(sarr, (jint) 0, data->data_len, (jshort *) data->data);
        jniEnv->CallVoidMethod(obj, onRecvVideoDataM, sarr, data->ntp_time_local);
    jniEnv->DeleteLocalRef(sarr);
    g_JavaVM->DetachCurrentThread();
}


void OnRecvVideoData(void *observer, video_decode_struct *data) {
    jobject obj = (jobject) observer;
    JNIEnv* jniEnv = NULL;
    g_JavaVM->AttachCurrentThread(&jniEnv, NULL);
    jclass cls = jniEnv->GetObjectClass(obj);
    jmethodID onRecvVideoDataM = jniEnv->GetMethodID(cls, "onRecvVideoData", "([BIJJ)V");
    jniEnv->DeleteLocalRef(cls);
    jbyteArray barr = jniEnv->NewByteArray(data->data_len);
        if (barr == NULL) return;
        jniEnv->SetByteArrayRegion(barr, (jint) 0, data->data_len, (jbyte *) data->data);
        jniEnv->CallVoidMethod(obj, onRecvVideoDataM, barr, 0,
                               data->ntp_time_local, data->ntp_time_local);
    jniEnv->DeleteLocalRef(barr);
    g_JavaVM->DetachCurrentThread();
}

extern "C" void
airplay_play_url(const char *buffer, float positon)
{
    LOGD("%s", __FUNCTION__ );
    JNIEnv* jniEnv = NULL;
    g_JavaVM->AttachCurrentThread(&jniEnv, NULL);
    J4AC_AirplayLogic__airplayPlayUrl__withCString(jniEnv,(const char *)buffer,positon);
}

extern "C" void
audio_process(void *cls, raop_ntp_t *ntp, audio_decode_struct *data) {
    OnRecvAudioData(cls, data);
}

extern "C" void
audio_set_volume(void *cls, float volume) {

}

extern "C" void
video_process(void *cls,  raop_ntp_t *ntp,video_decode_struct *data) {
    OnRecvVideoData(cls, data);
}

extern "C" void
log_callback(void *cls, int level, const char *msg) {
    switch (level) {
        case LOGGER_DEBUG: {
            LOGD("%s", msg);
            break;
        }
        case LOGGER_WARNING: {
            LOGW("%s", msg);
            break;
        }
        case LOGGER_INFO: {
            LOGI("%s", msg);
            break;
        }
        case LOGGER_ERR: {
            LOGE("%s", msg);
            break;
        }
        default:break;
    }

}

extern "C" void conn_init (void *cls) {
    open_connections++;
    LOGD("Open connections: %i", open_connections);
    //video_renderer_update_background(1);
}

extern "C" void conn_destroy (void *cls) {
    //video_renderer_update_background(-1);
    open_connections--;
    LOGD("Open connections: %i", open_connections);
    if (open_connections == 0) {

    }
}

extern "C" void conn_reset (void *cls, int timeouts, bool reset_video) {
    LOGI("***ERROR lost connection with client (network problem?)");
    printf("reset_video %d\n",(int) reset_video);

}

extern "C" void conn_teardown(void *cls, bool *teardown_96, bool *teardown_110) {
    LOGD("conn_teardown");
}
extern "C" void video_pause(void *cls) {
    LOGD("video_pause");
}

extern "C" void video_resume (void *cls) {
    LOGD("video_resume");
}


extern "C" void audio_flush (void *cls) {
    LOGD("audio_flush");
}

extern "C" void video_flush (void *cls) {
    LOGD("video_flush");
}

extern "C" void video_set_codec(void *cls, video_codec_t codec) {
    LOGD("video_set_codec");
}

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_JavaVM = vm;
    JNIEnv* env = NULL;
    int retval;
    if (g_JavaVM->GetEnv( (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    retval = J4A_LoadAll__catchAll(env);
    JNI_CHECK_RET(retval == 0, env, NULL, NULL, -1);
    return JNI_VERSION_1_6;
}

extern "C" void audio_get_format (void *cls, unsigned char *ct, unsigned short *spf, bool *usingScreen, bool *isMedia, uint64_t *audioFormat) {

}
extern "C" void video_report_size(void *cls, float *width_source, float *height_source, float *width, float *height) {

}
extern "C" void audio_set_metadata(void *cls, const void *buffer, int buflen) {

}

extern "C" void audio_set_coverart(void *cls, const void *buffer, int buflen) {

}
extern "C" void audio_set_progress(void *cls, unsigned int start, unsigned int curr, unsigned int end) {

}
extern "C" void report_client_request(void *cls, char *deviceid, char * model, char *name, bool * admit) {

}
extern "C" void display_pin(void *cls, char *pin) {

}

extern "C" void register_client(void *cls, const char *device_id, const char *client_pk, const char *client_name) {

}

extern "C" bool check_register(void *cls, const char *client_pk) {
        /* we are not maintaining a list of registered clients */
        return true;

}


        static int start_dnssd(const char* hw_addr, const char*  name) {
    int dnssd_error;
    int require_pw = 0;
    if (dnssd) {
        LOGE("start_dnssd error: dnssd != NULL");
        return 2;
    }
    dnssd = dnssd_init(hw_addr, strlen(hw_addr), name,strlen(name), &dnssd_error, require_pw);
    if (dnssd_error) {
        LOGE("Could not initialize dnssd library!: error %d", dnssd_error);
        return 1;
    }

    /* after dnssd starts, reset the default feature set here
     * (overwrites features set in dnssdint.h)
     * default: FEATURES_1 = 0x5A7FFEE6, FEATURES_2 = 0 */

    dnssd_set_airplay_features(dnssd,  0, 0); // AirPlay video supported
    dnssd_set_airplay_features(dnssd,  1, 1); // photo supported
    dnssd_set_airplay_features(dnssd,  2, 1); // video protected with FairPlay DRM
    dnssd_set_airplay_features(dnssd,  3, 0); // volume control supported for videos

    dnssd_set_airplay_features(dnssd,  4, 0); // http live streaming (HLS) supported
    dnssd_set_airplay_features(dnssd,  5, 1); // slideshow supported
    dnssd_set_airplay_features(dnssd,  6, 1); //
    dnssd_set_airplay_features(dnssd,  7, 1); // mirroring supported

    dnssd_set_airplay_features(dnssd,  8, 0); // screen rotation  supported
    dnssd_set_airplay_features(dnssd,  9, 1); // audio supported
    dnssd_set_airplay_features(dnssd, 10, 1); //
    dnssd_set_airplay_features(dnssd, 11, 1); // audio packet redundancy supported

    dnssd_set_airplay_features(dnssd, 12, 1); // FaiPlay secure auth supported
    dnssd_set_airplay_features(dnssd, 13, 1); // photo preloading  supported
    dnssd_set_airplay_features(dnssd, 14, 1); // Authentication bit 4:  FairPlay authentication
    dnssd_set_airplay_features(dnssd, 15, 1); // Metadata bit 1 support:   Artwork

    dnssd_set_airplay_features(dnssd, 16, 1); // Metadata bit 2 support:  Soundtrack  Progress
    dnssd_set_airplay_features(dnssd, 17, 1); // Metadata bit 0 support:  Text (DAACP) "Now Playing" info.
    dnssd_set_airplay_features(dnssd, 18, 1); // Audio format 1 support:
    dnssd_set_airplay_features(dnssd, 19, 1); // Audio format 2 support: must be set for AirPlay 2 multiroom audio

    dnssd_set_airplay_features(dnssd, 20, 1); // Audio format 3 support: must be set for AirPlay 2 multiroom audio
    dnssd_set_airplay_features(dnssd, 21, 1); // Audio format 4 support:
    dnssd_set_airplay_features(dnssd, 22, 1); // Authentication type 4: FairPlay authentication
    dnssd_set_airplay_features(dnssd, 23, 0); // Authentication type 1: RSA Authentication

    dnssd_set_airplay_features(dnssd, 24, 0); //
    dnssd_set_airplay_features(dnssd, 25, 1); //
    dnssd_set_airplay_features(dnssd, 26, 0); // Has Unified Advertiser info
    dnssd_set_airplay_features(dnssd, 27, 1); // Supports Legacy Pairing

    dnssd_set_airplay_features(dnssd, 28, 1); //
    dnssd_set_airplay_features(dnssd, 29, 0); //
    dnssd_set_airplay_features(dnssd, 30, 1); // RAOP support: with this bit set, the AirTunes service is not required.
    dnssd_set_airplay_features(dnssd, 31, 0); //


    /*  bits 32-63: see  https://emanualcozzi.net/docs/airplay2/features
    dnssd_set_airplay_features(dnssd, 32, 0); // isCarPlay when ON,; Supports InitialVolume when OFF
    dnssd_set_airplay_features(dnssd, 33, 0); // Supports Air Play Video Play Queue
    dnssd_set_airplay_features(dnssd, 34, 0); // Supports Air Play from cloud (requires that bit 6 is ON)
    dnssd_set_airplay_features(dnssd, 35, 0); // Supports TLS_PSK

    dnssd_set_airplay_features(dnssd, 36, 0); //
    dnssd_set_airplay_features(dnssd, 37, 0); //
    dnssd_set_airplay_features(dnssd, 38, 0); //  Supports Unified Media Control (CoreUtils Pairing and Encryption)
    dnssd_set_airplay_features(dnssd, 39, 0); //

    dnssd_set_airplay_features(dnssd, 40, 0); // Supports Buffered Audio
    dnssd_set_airplay_features(dnssd, 41, 0); // Supports PTP
    dnssd_set_airplay_features(dnssd, 42, 0); // Supports Screen Multi Codec (allows h265 video)
    dnssd_set_airplay_features(dnssd, 43, 0); // Supports System Pairing

    dnssd_set_airplay_features(dnssd, 44, 0); // is AP Valeria Screen Sender
    dnssd_set_airplay_features(dnssd, 45, 0); //
    dnssd_set_airplay_features(dnssd, 46, 0); // Supports HomeKit Pairing and Access Control
    dnssd_set_airplay_features(dnssd, 47, 0); //

    dnssd_set_airplay_features(dnssd, 48, 0); // Supports CoreUtils Pairing and Encryption
    dnssd_set_airplay_features(dnssd, 49, 0); //
    dnssd_set_airplay_features(dnssd, 50, 0); // Metadata bit 3: "Now Playing" info sent by bplist not DAACP test
    dnssd_set_airplay_features(dnssd, 51, 0); // Supports Unified Pair Setup and MFi Authentication

    dnssd_set_airplay_features(dnssd, 52, 0); // Supports Set Peers Extended Message
    dnssd_set_airplay_features(dnssd, 53, 0); //
    dnssd_set_airplay_features(dnssd, 54, 0); // Supports AP Sync
    dnssd_set_airplay_features(dnssd, 55, 0); // Supports WoL

    dnssd_set_airplay_features(dnssd, 56, 0); // Supports Wol
    dnssd_set_airplay_features(dnssd, 57, 0); //
    dnssd_set_airplay_features(dnssd, 58, 0); // Supports Hangdog Remote Control
    dnssd_set_airplay_features(dnssd, 59, 0); // Supports AudioStreamConnection setup

    dnssd_set_airplay_features(dnssd, 60, 0); // Supports Audo Media Data Control
    dnssd_set_airplay_features(dnssd, 61, 0); // Supports RFC2198 redundancy
    */

    /* needed for HLS video support */
    dnssd_set_airplay_features(dnssd, 0, (int) 1);
    dnssd_set_airplay_features(dnssd, 4, (int) 1);
    // not sure about this one (bit 8, screen rotation supported):
    //dnssd_set_airplay_features(dnssd, 8, (int) hls_support);

    /* needed for h265 video support */
    dnssd_set_airplay_features(dnssd, 42, (int) 0);

    /* bit 27 of Features determines whether the AirPlay2 client-pairing protocol will be used (1) or not (0) */
    dnssd_set_airplay_features(dnssd, 27, (int) 0);
    return 0;
}


static int register_dnssd(int raop_port,int airplay_port) {
    int dnssd_error;
    uint64_t features;

    if ((dnssd_error = dnssd_register_raop(dnssd, raop_port))) {
        if (dnssd_error == -65537) {
            LOGE("No DNS-SD Server found (DNSServiceRegister call returned kDNSServiceErr_Unknown)");
        } else if (dnssd_error == -65548) {
            LOGE("DNSServiceRegister call returned kDNSServiceErr_NameConflict");
            LOGI("Is another instance of %s running with the same DeviceID (MAC address) or using same network ports?",
                 DEFAULT_NAME);
            LOGI("Use options -m ... and -p ... to allow multiple instances of %s to run concurrently", DEFAULT_NAME);
        } else {
            LOGE("dnssd_register_raop failed with error code %d\n"
                 "mDNS Error codes are in range FFFE FF00 (-65792) to FFFE FFFF (-65537) "
                 "(see Apple's dns_sd.h)", dnssd_error);
        }
        return -3;
    }
    if ((dnssd_error = dnssd_register_airplay(dnssd, airplay_port))) {
        LOGE("dnssd_register_airplay failed with error code %d\n"
             "mDNS Error codes are in range FFFE FF00 (-65792) to FFFE FFFF (-65537) "
             "(see Apple's dns_sd.h)", dnssd_error);
        return -4;
    }

    LOGD("register_dnssd: advertised AirPlay service with \"Features\" code = 0x%llX",
         dnssd_get_airplay_features(dnssd));
    return 0;
}
extern "C" void export_dacp(void *cls, const char *active_remote, const char *dacp_id) {

}

extern "C" void video_reset(void *cls) {
    LOGD("video_reset");
}
extern "C" void on_video_play(void *cls, const char* location, const float start_position) {
    /* start_position needs to be implemented */
    LOGD("on_video_play location=%s,start_position=%f",location,start_position);
    airplay_play_url(location,start_position);
}
extern "C" void on_video_scrub(void *cls, const float position) {
    LOGI("on_video_scrub: position = %7.5f\n", position);
}

extern "C" void on_video_rate(void *cls, const float rate) {
    LOGI("on_video_rate = %7.5f\n", rate);
}

extern "C" void on_video_stop(void *cls) {
    LOGI("on_video_stop\n");
}
bool video_get_playback_info(double *duration, double *position, float *rate) {
    *duration = 0.0;
    *position = -1.0;
    *rate = 0.0f;
    return true;
}
extern "C" void on_video_acquire_playback_info (void *cls, playback_info_t *playback_info) {

    int buffering_level;
    LOGD("on_video_acquire_playback info\n");
    bool still_playing = video_get_playback_info(&playback_info->duration, &playback_info->position,
                                                 &playback_info->rate);
    LOGD("on_video_acquire_playback info done\n");
    if (!still_playing) {
        LOGI(" video has finished, %f", playback_info->position);
        playback_info->position = -1.0;
        playback_info->duration = -1.0;
//        video_renderer_stop();
    }

}
extern "C" JNIEXPORT jlong JNICALL
Java_com_smart_home_RaopServer_start(JNIEnv *env, jobject object) {
    const char *server_hw_addr = "aa:bb:cc:dd:ee:ff";
    const char *server_name = "Debug6";
    if (start_dnssd(server_hw_addr, server_name)) {
        LOGD("start_dnssd fail");
        return 0;
    } else{
        LOGD("start_dnssd ok");
    }
        raop_t *raop;
        raop_callbacks_t raop_cbs;
        memset(&raop_cbs, 0, sizeof(raop_cbs));
        raop_cbs.cls = (void *) env->NewGlobalRef(object);
        raop_cbs.conn_init = conn_init;
        raop_cbs.conn_destroy = conn_destroy;
        raop_cbs.conn_reset = conn_reset;
        raop_cbs.conn_teardown = conn_teardown;
        raop_cbs.audio_process = audio_process;
        raop_cbs.video_process = video_process;
        raop_cbs.audio_flush = audio_flush;
        raop_cbs.video_flush = video_flush;
        raop_cbs.video_pause = video_pause;
        raop_cbs.video_resume = video_resume;
    raop_cbs.audio_set_volume = audio_set_volume;
    raop_cbs.audio_get_format = audio_get_format;
    raop_cbs.video_report_size = video_report_size;
    raop_cbs.audio_set_metadata = audio_set_metadata;
    raop_cbs.audio_set_coverart = audio_set_coverart;
    raop_cbs.audio_set_progress = audio_set_progress;
    raop_cbs.report_client_request = report_client_request;
    raop_cbs.display_pin = display_pin;
    raop_cbs.register_client = register_client;
    raop_cbs.check_register = check_register;
    raop_cbs.export_dacp = export_dacp;
    raop_cbs.video_reset = video_reset;
    raop_cbs.video_set_codec = video_set_codec;
    raop_cbs.on_video_play = on_video_play;
    raop_cbs.on_video_scrub = on_video_scrub;
    raop_cbs.on_video_rate = on_video_rate;
    raop_cbs.on_video_stop = on_video_stop;
    raop_cbs.on_video_acquire_playback_info = on_video_acquire_playback_info;


    raop = raop_init(&raop_cbs);
        if (raop == NULL) {
            LOGE("raop = NULL");
            return 0;
        } else {
            LOGD("raop init success");
        }

        raop_set_log_callback(raop, log_callback, NULL);
        raop_set_log_level(raop, LOGGER_DEBUG);
        if (raop_init2(raop, 0, server_hw_addr, "")) {
            LOGE("Error initializing raop (2)!");
            free(raop);
            return -1;
        }

        raop_set_plist(raop, "width", 960);
        raop_set_plist(raop, "height", 540);
        raop_set_plist(raop, "refreshRate", 60);
        raop_set_plist(raop, "maxFPS", 30);
        raop_set_plist(raop, "overscanned", 0);
        raop_set_plist(raop, "hls", 1);
        unsigned short port = 0;
        unsigned short airplayPort = 0;
        raop_start(raop, &port);
        raop_set_port(raop, port);
        if (dnssd) {
            LOGD("raop_set_dnssd 0");
            raop_set_dnssd(raop, dnssd);
            LOGD("raop_set_dnssd 1");
        } else {
            LOGE("raop_set failed to set dnssd");
            return -2;
        }
        LOGD("raop port = % d", raop_get_port(raop));
        if (register_dnssd(raop_get_port(raop),raop_get_port(raop))){
            LOGD("register_dnssd fail");
            return 0;
        } else{
            LOGD("register_dnssd ok");
        }
        return (jlong) (void *) raop;

}

extern "C" JNIEXPORT jint JNICALL
Java_com_smart_home_RaopServer_getPort(JNIEnv* env, jobject object, jlong opaque) {
    raop_t *raop = (raop_t *) (void *) opaque;
    return raop_get_port(raop);
}

extern "C" JNIEXPORT void JNICALL
Java_com_smart_home_RaopServer_stop(JNIEnv* env, jobject object, jlong opaque) {
    raop_t *raop = (raop_t *) (void *) opaque;
    jobject obj = (jobject) raop_get_callback_cls(raop);
    raop_destroy(raop);
    env->DeleteGlobalRef(obj);

}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_smart_home_RaopServer_start_1raop_1server(JNIEnv *env, jobject thiz) {
    return 0;
}

