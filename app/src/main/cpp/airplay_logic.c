//
// Created by Lenovo on 2024/2/20.
//
#include "airplay_logic.h"
#include "log.h"

typedef struct J4AC_com_smart_home_AirplayLogic {
    jclass id;
    jmethodID method_airplayPlayUrl;
    jmethodID method_tearDown;
} J4AC_com_smart_home_AirplayLogic;
static J4AC_com_smart_home_AirplayLogic class_J4AC_com_smart_home_AirplayLogic;

void J4A_loadClass__J4AC_com_smart_home_AirplayLogic__airplayPlayUrl(JNIEnv *env, jstring url,jfloat position)
{
    (*env)->CallStaticVoidMethod(env, class_J4AC_com_smart_home_AirplayLogic.id, class_J4AC_com_smart_home_AirplayLogic.method_airplayPlayUrl, url, position);
}

void J4A_loadClass__J4AC_com_smart_home_AirplayLogic__airplayPlayUrl__catchAll(JNIEnv *env, jstring url,jfloat position)
{
    J4A_loadClass__J4AC_com_smart_home_AirplayLogic__airplayPlayUrl(env, url,position);
    J4A_ExceptionCheck__catchAll(env);
}

void J4A_loadClass__J4AC_com_smart_home_AirplayLogic__tearDown(JNIEnv *env)
{
    (*env)->CallStaticVoidMethod(env, class_J4AC_com_smart_home_AirplayLogic.id, class_J4AC_com_smart_home_AirplayLogic.method_tearDown);
}


void J4A_loadClass__J4AC_com_smart_home_AirplayLogic__airplayPlayUrl__withCString(JNIEnv *env, const char * buffer,float position){
    LOGD("%s", __FUNCTION__ );
    jstring url= NULL;
    url = (*env)->NewStringUTF(env, buffer);
    if (J4A_ExceptionCheck__throwAny(env) || !url)
        goto fail;

    (*env)->CallStaticVoidMethod(env, class_J4AC_com_smart_home_AirplayLogic.id, class_J4AC_com_smart_home_AirplayLogic.method_airplayPlayUrl, url, position);
    if (J4A_ExceptionCheck__throwAny(env)) {
        goto fail;
    }
fail:
    J4A_DeleteLocalRef__p(env, &url);
    return ;
}
int J4A_loadClass__J4AC_com_smart_home_AirplayLogic(JNIEnv *env)
{
    int         ret                   = -1;
    const char *J4A_UNUSED(name)      = NULL;
    const char *J4A_UNUSED(sign)      = NULL;
    jclass      J4A_UNUSED(class_id)  = NULL;
    int         J4A_UNUSED(api_level) = 0;

    if (class_J4AC_com_smart_home_AirplayLogic.id != NULL)
        return 0;

    sign = "com/smart/home/AirplayLogic";
    class_J4AC_com_smart_home_AirplayLogic.id = J4A_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_J4AC_com_smart_home_AirplayLogic.id == NULL)
        goto fail;

    class_id = class_J4AC_com_smart_home_AirplayLogic.id;
    name     = "airplayPlayUrl";
    sign     = "(Ljava/lang/String;F)V";
    class_J4AC_com_smart_home_AirplayLogic.method_airplayPlayUrl = J4A_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_J4AC_com_smart_home_AirplayLogic.method_airplayPlayUrl == NULL)
        goto fail;

    class_id = class_J4AC_com_smart_home_AirplayLogic.id;
    name     = "tearDown";
    sign     = "()V";
    class_J4AC_com_smart_home_AirplayLogic.method_tearDown = J4A_GetStaticMethodID__catchAll(env, class_id, name, sign);
    if (class_J4AC_com_smart_home_AirplayLogic.method_tearDown == NULL)
        goto fail;


    J4A_ALOGD("J4ALoader: OK: '%s' loaded\n", "com.smart.home.AirplayLogic");
    ret = 0;
    fail:
    return ret;
}
