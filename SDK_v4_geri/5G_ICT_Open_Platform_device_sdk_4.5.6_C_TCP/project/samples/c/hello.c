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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "iotmakers_dll_impl.h"

/**
EXPORT int __stdcall ImInit(int debug_level);
EXPORT int __stdcall ImRelease(int hndl);

EXPORT int __stdcall ImConnectTo(int hndl, char* ec_ip, int ec_port);
EXPORT int __stdcall ImConnect(int hndl);
EXPORT int __stdcall ImDisconnect(int hndl);
EXPORT int __stdcall ImTurnCircuitBreakerOff(int hndl);
EXPORT int __stdcall ImTurnResponseWaitOff(int hndl);

EXPORT int __stdcall ImAuthDevice(int hndl, char *im_dev_id,	char *im_dev_pw, char *im_dev_gw);
EXPORT int __stdcall ImPoll(int hndl);
EXPORT int __stdcall ImMSleep(int hndl, int msec);

EXPORT int __stdcall ImSendData(int hndl, char *json_str, char *resource_name);
EXPORT int __stdcall ImSendDataOnDev(int hndl, char *json_str, char *resource_name, char *dev_id);

EXPORT void __stdcall ImSet_strdata_handler(int hndl, char*(*func)(void *cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value));
EXPORT void __stdcall ImSet_numdata_handler(int double, void(*func)(void *cli, char *dev_id, char *resource_name, char *prop_name, double prop_value));

EXPORT void __stdcall ImSet_intdata_handler(int hndl, int(*func)(void *cli, char *dev_id, char *resource_name, char *prop_name, int prop_value));
EXPORT void __stdcall ImSet_floatdata_handler(int hndl, double(*func)(void *cli, char *dev_id, char *resource_name, char *prop_name, double prop_value));
EXPORT void __stdcall ImSet_booldata_handler(int hndl, int(*func)(void *cli, char *dev_id, char *resource_name, char *prop_name, int prop_value));
*/



static char g_report_str[250];

 char* example_if525_request_handler_for_string(void *cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%s]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	
	sprintf(g_report_str, "{\"%s\":\"%s\"}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_integer(void *cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%d}", prop_name, prop_value);

	return prop_value;
}

double example_if525_request_handler_for_float(void *cli, char *dev_id, char *resource_name, char *prop_name, double prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%g]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%g}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_boolean(void *cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");


	sprintf(g_report_str, "{\"%s\":%s}", prop_name, (prop_value==1)?"true":"false");

	return prop_value;
}

int example_if525_handler_on_end_of_control(void *cli, char *dev_id, char *resource_name, void *report_body)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s] End of control\n", dev_id, resource_name);
	printf("====================================\n");

	// 응답용 제어Tag별 결과값을 세팅하는 예제
	if ( report_body != NULL )	{

		// Report in JSON String
		if ( strlen(g_report_str) > 0 )
		{
			ImSetReportDataOnDev(report_body, g_report_str, resource_name, dev_id);
			g_report_str[0] = '\0';
		} 
		else
		{
			char *json_str = "{\"command\":\"OFF\"}";
			ImSetReportDataOnDev(report_body, json_str, resource_name, dev_id);
		}
	}

	// 응답용 제어결과 코드를 리턴
	return (100); // code: 100=OK, 200=ERR
}



/** 
 *   Usage : simple_app {ec_ip} {ec_port} {dev_id} {dev_pw} {dev_gw}
 */  
int main(int argc, char *argv[])
{
  int ret = 0;

  if ( argc != 6 )  {
    printf("Usage : %s {ec_ip} {ec_port} {dev_id} {dev_pw} {dev_gw}\n", argv[0]);
    exit(0);
  }

  char  *ec_ip = argv[1];
  int  ec_port = atoi(argv[2]);
  char *dev_id = argv[3];
  char *dev_pw = argv[4];
  char *svc_gw = argv[5];

  printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n", 
                        ec_ip, ec_port, dev_id, dev_pw, svc_gw);


  /********************************************************
  *  IoTMakers 디바이스 클라이언트 핸들러
  ********************************************************/

  ///////////////////////////////////////////////////
  // IoTMakers Device Init; 1=ERROR, 2=INFO, 3=DEBUG
  ///////////////////////////////////////////////////
  int log_level = 3;
  
  int hndl = ImInit(log_level);
  if ( hndl < 0 )		{
    printf("fail ImInit()\n");
    return 0;
  }
  
  ///////////////////////////////////////////////////
  // Register Device Control Handler
  ///////////////////////////////////////////////////
  ImSet_strdata_handler(hndl, example_if525_request_handler_for_string);
  ImSet_intdata_handler(hndl, example_if525_request_handler_for_integer);
  ImSet_floatdata_handler(hndl, example_if525_request_handler_for_float);
  ImSet_booldata_handler(hndl, example_if525_request_handler_for_boolean);
  ImSet_endOfControl_handler(hndl, example_if525_handler_on_end_of_control);

  
  ///////////////////////////////////////////////////
  // Connect to Server
  ///////////////////////////////////////////////////
  if ( ImConnectTo(hndl, ec_ip, ec_port) < 0 )	{
  	return 0;
  }
  
  ///////////////////////////////////////////////////
  // Auth Device
  ///////////////////////////////////////////////////
  if ( ImAuthDevice(hndl, dev_id, dev_pw, svc_gw) < 0 )	{
  	return 0;
  }
  
  while (1==1)
  {
  	///////////////////////////////////////////////////
  	// Send Sensing data in JSON
  	///////////////////////////////////////////////////
#if 1
	char* jdata = "{\"Temp\":25.5, \"status\":\"hello\"}";
#else
    char *jdata = "{\"Temp\":25.5, "
					"\"Hum\":50, "
					"\"status\":\"hello\", "
					"\"TRUE\":true, "
					"\"FALSE\":false, "
					"\"DATE\":\"2020-10-10 10:10:10.100\", "

					"\"ARR_INT\":[0,1,2,3,4], "
					"\"ARR_FLT\":[0.5,1.5,2.5,3.5,4.5], "
					"\"ARR_STR\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"ARR_DATE\":[\"2020-10-11 10:10:10.100\", \"2020-10-12 10:10:10.100\"], "
					"\"ARR_BOOL\":[true, true, false]"
					"}";
#endif

	char *resource_name = "test";

	//////////////////////////////////////
	// 센싱데이터 전송 예
	ret = ImSendData(hndl, jdata, resource_name);

	/*
	//////////////////////////////////////
	// 하위장치, 그룹태그명 지정예제
	char *subDevId = NULL;
	char *resource_name = "testGroup";
	*/
	//ret = ImSendDataOnDev(hndl, jdata, subDevId, resource_name);
			
  	if ( ret < 0 )			{
  		break;
  	}

    ImMSleep(hndl, 1*1000);

  	///////////////////////////////////////////////////
  	// NetIO polling
  	///////////////////////////////////////////////////
  	if ( ImPoll(hndl) < 0 )			{
  		break;
  	}
  	
  }
  
  ///////////////////////////////////////////////////
  // Disconnect
  ///////////////////////////////////////////////////
  ImDisconnect(hndl);
  
	///////////////////////////////////////////////////
	// Release
	///////////////////////////////////////////////////
	ImRelease(hndl);
   
  return 0;
}



