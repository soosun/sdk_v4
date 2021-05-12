#include <jni.h>

#ifndef __IOTMAKERS_JNI__

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImInit(JNIEnv *, jobject, jint debug_level);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImRelease(JNIEnv *, jobject, int hndl);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnectTo(JNIEnv *, jobject, int hndl, jstring  ec_ip, int ec_port);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImConnect(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImDisconnect(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnCircuitBreakerOff(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImTurnResponseWaitOff(JNIEnv *, jobject, int hndl);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImAuthDevice(JNIEnv *, jobject, int hndl, jstring im_dev_id, jstring im_dev_pw, jstring im_dev_gw);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImPoll(JNIEnv *, jobject, int hndl);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImMSleep(JNIEnv *, jobject, int hndl, int msec);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSendData(JNIEnv *, jobject, int hndl, jstring json_str, jstring resource_name);
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSendDataOnDev(JNIEnv *, jobject, int hndl, jstring json_str, jstring resource_name, jstring dev_id);

JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetControlCallBack(JNIEnv *, jobject, int hndl);

#if 1 // 2020-11-16
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetReportDataOnDev(JNIEnv *, jobject, jlong rep_body, jstring json_str, jstring resource_name, jstring dev_id);
#else
	// be deprecated
JNIEXPORT int JNICALL Java_com_kt_iotmakers_device_IoTMakersDeviceClient_ImSetReportForControl(JNIEnv *, jobject, jlong rep_body, jstring resource_name, jstring json_str);
#endif

#ifdef __cplusplus
}
#endif

#endif