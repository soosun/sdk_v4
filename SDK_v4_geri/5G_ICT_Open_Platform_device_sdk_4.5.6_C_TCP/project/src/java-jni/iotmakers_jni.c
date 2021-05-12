/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

/* iotmakers_dll_impl.c */

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "client/client.h"
#include "util/util.h"
#include "kt_iot_log.h"

#include "iotmakers_jni.h"


///////////////////////////////////////////////////
//  SESSION POOL STUFF

#define IM_SESS_MAX   128

static im_client_tPtr sess_pool[IM_SESS_MAX];

static int sesspool_alloc()
{
  int sess_idx;
  
  for (sess_idx = 0; sess_idx < IM_SESS_MAX; sess_idx++)  {
    if ( sess_pool[sess_idx] == NULL )    {
      sess_pool[sess_idx] = malloc(sizeof(im_client_t));
      return sess_idx;
    }
  }

  ERR ("sess_pool is full, max = %d", IM_SESS_MAX);
  return -1;
}

static void sesspool_free(int hndl)
{
  if ( hndl < 0 || hndl >= IM_SESS_MAX ) {
    ERR ("hndl is out of %d", IM_SESS_MAX);
    return;
  }

  im_client_tPtr sess = sess_pool[hndl];
  if ( sess != NULL )  {
    free(sess);
    sess_pool[hndl] = NULL;
  }
}

static im_client_tPtr sesspool_sess(int hndl)
{
  if ( hndl < 0 || hndl >= IM_SESS_MAX ) {
    ERR ("hndl is out of %d", IM_SESS_MAX);
    return NULL;
  }

  if ( sess_pool[hndl] == NULL ) {
    ERR("no sess found in the sess_pool");
    return NULL;
  }

  return  sess_pool[hndl];
}

//  SESSION POOL STUFF
///////////////////////////////////////////////////


static char* getStringUTFChars(JNIEnv *env, jstring jstr)
{
#ifdef __cplusplus
    char *c_str = (char*)(env)->GetStringUTFChars(jstr, NULL);
#else
    char *c_str = (char*)(*env)->GetStringUTFChars( env, jstr, NULL ) ;
#endif
    return c_str;
}


static void releaseStringUTFChars(JNIEnv *env, char* c_str, jstring jstr)
{
#ifdef __cplusplus
    (env)->ReleaseStringUTFChars(ec_ip, c_str);
#else
    (*env)->ReleaseStringUTFChars(env, jstr, c_str);
#endif
}


/**
Calling Java Class Methods from C with JNI
    http://hildstrom.com/projects/jni/index.html
*/


static jclass findJavaClass(JNIEnv* env, char *class_path)
{
    jclass clz = (*env)->FindClass(env, class_path);
    if ( clz == NULL )    {
        ERR("java class IoTMakersDeviceClient Not Found");
        return NULL;
    }
    return clz;
}

static jmethodID findJavaStaticMethod(JNIEnv* env, jclass clz, char* method_name, char* method_sig)
{
    // find a static method
    jmethodID method = (*env)->GetStaticMethodID(env, clz, method_name, method_sig);
    if ( method == NULL )    {
        ERR("java method callback_method Not Found");
        return NULL;
    }
    return method;
}

static jmethodID findJavaMethod(JNIEnv* env, jclass clz, char* method_name, char* method_sig)
{
    // find a non-static method
    jmethodID method = (*env)->GetMethodID(env, clz, method_name, method_sig);
    if ( method == NULL )    {
        ERR("java method callback_method Not Found");
        return NULL;
    }
    return method;
}

#if 0
JavaVM * glpVM = NULL;

static jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    jint result = -1 ;
    JNIEnv* env = NULL ;
    jclass cls ;

    if ( (*vm)->GetEnv(vm,  (void**)&env, JNI_VERSION_1_4) != JNI_OK ) {
        INF("GetEnv failed.\n" ) ;
        goto error ;
    }

    result = JNI_VERSION_1_4 ;
    glpVM = vm ;

error:
    return result ;
}
#endif


static double __if525_numdata_handler(im_client_tPtr cli, char *devid, char *resource_name, char *prop_name, double numval)
{
	// !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], numval=[%g]\n", devid, prop_name, numval);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("JNIEnv is NULL");
        return (double)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found\n");
        return (double)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "onControlNumberData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;D)D");
    if ( method == NULL )    {
        printf("%s(): java method onControlNumberData Not Found\n", __FUNCTION__);
        return (double)-1;
    }


    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, prop_name); 
    jstring param3 = (*env)->NewStringUTF(env, resource_name); 
    jdouble param4 = numval; 
    jdouble retVal = 0;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallDoubleMethod(env, jobj, method, param1, param2, param3, param4);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (double)retVal;
}

static char* __if525_strdata_handler(im_client_tPtr cli, char *devid, char *resource_name, char *prop_name, char *strval)
{
    // !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], strval=[%s]\n", devid, prop_name, strval);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("%s(): java JNIEnv is NULL");
        return NULL;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found");
        return NULL;
    }

    jmethodID method = findJavaMethod(env, clz, "onControlStringData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if ( method == NULL )    {
        ERR("Java method onControlStringData Not Found");
        return NULL;
    }


    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, resource_name); 
    jstring param3 = (*env)->NewStringUTF(env, prop_name); 
    jstring param4 = (*env)->NewStringUTF(env, strval); 
    jstring retVal = NULL;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallObjectMethod(env, jobj, method, param1, param2, param3, param4);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	const char *strReturn = (*env)->GetStringUTFChars(env, retVal, 0);

	return strReturn;
}

static int __if525_intdata_handler(im_client_tPtr cli, char *devid, char *resource_name, char *prop_name, int intval)
{
    // !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], intval=[%d]\n", devid, prop_name, intval);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("%s(): java JNIEnv is NULL");
        return -1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found");
        return -1;
    }

    jmethodID method = findJavaMethod(env, clz, "onControlIntegerData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I");
    if ( method == NULL )    {
        ERR("Java method onControlIntegerData Not Found");
        return -1;
    }


    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, resource_name); 
    jstring param3 = (*env)->NewStringUTF(env, prop_name); 
    jint param4 = intval; 
    jint retVal = 0;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallIntMethod(env, jobj, method, param1, param2, param3, param4);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (int)retVal;
}


static double __if525_floatdata_handler(im_client_tPtr cli, char *devid, char *resource_name, char *prop_name, double fltval)
{
    // !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], fltval=[%g]\n", devid, prop_name, fltval);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("%s(): java JNIEnv is NULL");
        return (double)-1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found");
        return (double)-1;
    }

    jmethodID method = findJavaMethod(env, clz, "onControFloatData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;D)D");
    if ( method == NULL )    {
        ERR("Java method onControFloatData Not Found");
        return (double)-1;
    }


    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, resource_name); 
    jstring param3 = (*env)->NewStringUTF(env, prop_name); 
    jdouble param4 = fltval; 
    jfloat retVal = 0;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallFloatMethod(env, jobj, method, param1, param2, param3, param4);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (double)retVal;
}



static int __if525_booldata_handler(im_client_tPtr cli, char *devid, char *resource_name, char *prop_name, int intval)
{
    // !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], boolval=[%d]\n", devid, prop_name, intval);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("%s(): java JNIEnv is NULL");
        return -1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found");
        return -1;
    }

    jmethodID method = findJavaMethod(env, clz, "onControlBoolData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Z");
    if ( method == NULL )    {
        ERR("Java method onControlBoolData Not Found");
        return -1;
    }

    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, resource_name); 
    jstring param3 = (*env)->NewStringUTF(env, prop_name); 
    jboolean param4 = (intval==0)?0:1; 
    jboolean retVal = 0;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallBooleanMethod(env, jobj, method, param1, param2, param3, param4);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);
	(*env)->DeleteLocalRef(env, param3);

	return (retVal==0)?0:1;
}

static int __if525_done_handler(im_client_tPtr cli, char *devid, char *resource_name, void *report_body)
{
    // !!! USER CODE HERE
	DBG("devid=[%s], resource_name=[%s]\n", devid, resource_name);

    JNIEnv *env = cli->host_evn;
    if ( env == NULL )    {
        ERR("%s(): java JNIEnv is NULL");
        return -1;
    }

    jclass clz = findJavaClass(env, "com/kt/iotmakers/device/IoTMakersDeviceClient");
    if ( clz == NULL )    {
        ERR("Java Class IoTMakersDeviceClient Not Found");
        return -1;
    }

    jmethodID method = findJavaMethod(env, clz, "onEndOfControl", "(Ljava/lang/String;Ljava/lang/String;J)I");
    if ( method == NULL )    {
        ERR("Java method onEndOfControl Not Found");
        return -1;
    }

    jstring param1 = (*env)->NewStringUTF(env, devid); 
    jstring param2 = (*env)->NewStringUTF(env, resource_name); 
    jlong param3 = (jlong)report_body; 
    jint retVal = 0;

    jobject *jobj = cli->host_object;
    retVal = (*env)->CallBooleanMethod(env, jobj, method, param1, param2, param3);

	(*env)->DeleteLocalRef(env, param1);
	(*env)->DeleteLocalRef(env, param2);

	return retVal;
}

//
////////////////////////////////////////////////////////////




JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImInit(JNIEnv *env, jobject jobj, 
    jint debug_level)
{
    printf("%s(): \n", __FUNCTION__);

    int rc = 0;
 
  /********************************************************
  * session pool stuff
  ********************************************************/
  int sess_hndl = sesspool_alloc();
  if ( sess_hndl < 0 )  {
    return -1;
  }

  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  /********************************************************
  * 로그레벨 설정
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(debug_level);

  rc = im_client_init(pclient);
  if ( rc < 0 )  {
    return -1;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 reset
  ********************************************************/
  pclient->if525_handler_for_string = NULL;
  pclient->if525_handler_for_number = NULL;
  pclient->if525_handler_for_integer = NULL;
  pclient->if525_handler_for_float = NULL;
  pclient->if525_handler_for_boolean = NULL;


  // JNIEnv *env를 저장
  pclient->host_evn = env;
  pclient->host_object = jobj;

#if 0
  if ( (*glpVM)->AttachCurrentThread(glpVM, &env, NULL) != JNI_OK ) {
        ERR("%s: AttachCurrentThread() failed", __FUNCTION__);
        return -1;
  }
#endif
  return sess_hndl;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImRelease(JNIEnv *env, jobject jobj, 
    int hndl)
{
  sesspool_free(hndl);
 
#if 0
  if ( (*glpVM)->DetachCurrentThread(glpVM) != JNI_OK ) {
    ERR("%s: DetachCurrentThread() failed", __FUNCTION__);
  }
#endif

  return 0;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnectTo(JNIEnv *env, jobject jobj, 
    int hndl, jstring ec_ip, int ec_port)
{
    char *p_ec_ip = getStringUTFChars(env, ec_ip);

  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect_to(pclient, p_ec_ip, ec_port);


    releaseStringUTFChars(env, p_ec_ip, ec_ip);

  return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnect(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect(pclient);
	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImDisconnect(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

	im_disconnect(pclient);
  return 0;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnCircuitBreakerOff(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_circuit_breaker_off(pclient);
  return rc;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnResponseWaitOff(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_response_wait_off(pclient);
  return rc;
}



JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthDevice(JNIEnv *env, jobject jobj, 
    int hndl, jstring im_dev_id, jstring im_dev_pw, jstring im_dev_gw)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

    char *p_dev_id = getStringUTFChars(env, im_dev_id);
    char *p_dev_pw = getStringUTFChars(env, im_dev_pw);
    char *p_dev_gw = getStringUTFChars(env, im_dev_gw);

  int rc = im_auth_device(pclient, p_dev_id, p_dev_pw, p_dev_gw);

    releaseStringUTFChars(env, p_dev_id, im_dev_id);
    releaseStringUTFChars(env, p_dev_pw, im_dev_pw);
    releaseStringUTFChars(env, p_dev_gw, im_dev_gw);

    return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImPoll(JNIEnv *env, jobject jobj, 
    int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_poll(pclient);
	return rc;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImMSleep(JNIEnv *env, jobject jobj, 
    int hndl, int msec)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  im_msleep(msec);
  return 0;
}


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSendDataOnDev(JNIEnv *env, jobject jobj, 
    int hndl, jstring json_str, jstring resource_name, jstring dev_id)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = 0;

    char *p_json_str  = getStringUTFChars(env, json_str);
    char *p_resource_name = (resource_name==NULL)?NULL:getStringUTFChars(env, resource_name);
    char *p_dev_id    = (dev_id==NULL)?NULL:getStringUTFChars(env, dev_id);

    rc = im_send_data_on_dev(pclient, p_json_str, p_resource_name, p_dev_id);

    releaseStringUTFChars(env, p_json_str, json_str);

	if ( p_dev_id != NULL )	{
	    releaseStringUTFChars(env, p_dev_id, dev_id);
	}

	if ( p_resource_name != NULL )	{
		releaseStringUTFChars(env, p_resource_name, resource_name);
	}

	if ( rc < 0  )	{
		ERR("fail im_send_data_on_dev()\n");
		rc = -1;
	}

	return rc;
}



JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSendData(JNIEnv *env, jobject jobj, 
int hndl, jstring json_str, jstring resource_name)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  char *p_json_str = getStringUTFChars(env, json_str);
  char *p_resource_name = (resource_name==NULL)?NULL:getStringUTFChars(env, resource_name);

  int rc = im_send_data(pclient, p_json_str, p_resource_name);
  DBG("json_str=%s", p_json_str);

  releaseStringUTFChars(env, p_json_str, json_str);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_name);
  }
    
  if ( rc < 0  )	{
    ERR("fail im_send_data()\n");
    rc = -1;
  }

	return rc;
}

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetControlCallBack(JNIEnv *env, jobject jobj, 
int hndl)
{
  im_client_tPtr pclient = sesspool_sess(hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (숫자형/문자형)
  ********************************************************/
  pclient->if525_handler_for_string = __if525_strdata_handler;
  pclient->if525_handler_for_number = __if525_numdata_handler;
  pclient->if525_handler_for_integer = __if525_intdata_handler;
  pclient->if525_handler_for_float = __if525_floatdata_handler;
  pclient->if525_handler_for_boolean = __if525_booldata_handler;
  pclient->if525_handler_on_end_of_control = __if525_done_handler;

	return 0;
}


#if 1	// 2020-11-16
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetReportDataOnDev(JNIEnv *env, jobject jobj, 
jlong rep_body, jstring json_str, jstring resource_name, jstring dev_id)
{
  char *p_resource_name = (resource_name==NULL)?NULL:getStringUTFChars(env, resource_name);
  char *p_json_str = (json_str==NULL)?NULL:getStringUTFChars(env, json_str);
  char *p_dev_id = (dev_id==NULL)?NULL:getStringUTFChars(env, dev_id);

	int ret = im_set_report_data_on_dev((void*)rep_body, p_json_str, p_resource_name, p_dev_id);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_name);
  }
  if ( p_json_str != NULL )	{
    releaseStringUTFChars(env, p_json_str, json_str);
  }
  if ( p_dev_id != NULL )	{
    releaseStringUTFChars(env, p_dev_id, dev_id);
  }


	return ret;
}
#else
	// be deprecated
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetReportForControl(JNIEnv *env, jobject jobj, 
jlong rep_body, jstring resource_name, jstring json_str)
{
  char *p_resource_name = (resource_name==NULL)?NULL:getStringUTFChars(env, resource_name);
  char *p_json_str = (json_str==NULL)?NULL:getStringUTFChars(env, json_str);

	int ret = im_if525_rep_set_data_in_json((void*)rep_body, p_resource_name, p_json_str);

  if ( p_resource_name != NULL )	{
    releaseStringUTFChars(env, p_resource_name, resource_name);
  }
  if ( p_json_str != NULL )	{
    releaseStringUTFChars(env, p_json_str, json_str);
  }

	return ret;
}
#endif




JNIEXPORT void JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_sayHello(JNIEnv *env, jobject jobj, 
    jstring hello)
{
    char *p_hello = getStringUTFChars(env, hello);

    printf("%s(): say [%s]\n", __FUNCTION__, p_hello);

    releaseStringUTFChars(env, p_hello, hello);
    return;
}


