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

#include <stdio.h>
#include <stdlib.h>
#include "client/client.h"
#include "util/util.h"
#include "kt_iot_log.h"

#include "iotmakers_dll_impl.h"

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


#if 0
static void __if525_numdata_handler(im_client_tPtr cli, char *devid, char *prop_name, double numval)
{
	// !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], val=[%f]\n", devid, prop_name, numval);
}

static void __if525_strdata_handler(im_client_tPtr cli, char *devid, char *prop_name, char *strval)
{
	// !!! USER CODE HERE
	DBG("devid=[%s], prop_name=[%s], val=[%s]\n", devid, prop_name, strval);
}
#endif

int __stdcall ImInit(int debug_level)
{
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
  *  디바이스 제어수신 처리기 등록 (숫자형/문자형)
  ********************************************************/
  pclient->if525_handler_for_number = NULL;
  pclient->if525_handler_for_string = NULL;

  return sess_hndl;
}

int __stdcall ImRelease(int sess_hndl)
{
  sesspool_free(sess_hndl);
  return 0;
}

int __stdcall ImConnectTo(int sess_hndl, char* ec_ip, int ec_port)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect_to(pclient, ec_ip, ec_port);
  return rc;
}



int __stdcall ImTurnCircuitBreakerOff(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_circuit_breaker_off(pclient);
  return rc;
}


int __stdcall ImTurnResponseWaitOff(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_turn_response_wait_off(pclient);
  return rc;
}




int __stdcall ImConnect(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_connect(pclient);
	return rc;
}

int __stdcall ImDisconnect(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

	im_disconnect(pclient);
  return 0;
}

//////////////////////////////////
int __stdcall ImAuthDevice(int sess_hndl, char *im_dev_id,	char *im_dev_pw, char *im_dev_gw)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_auth_device(pclient, im_dev_id, im_dev_pw, im_dev_gw);
	return rc;
}

int __stdcall ImPoll(int sess_hndl)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_poll(pclient);
	return rc;
}

int __stdcall ImMSleep(int sess_hndl, int msec)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  im_msleep(msec);
  return 0;
}





//////////////////////////////////
int __stdcall ImSendDataOnDev(int sess_hndl, char *json_str, char *resource_name, char *dev_id)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = 0;

    rc = im_send_data_on_dev(pclient, json_str, resource_name, dev_id);

	if ( rc < 0  )	{
		ERR("fail im_send_data_on_dev()\n");
		rc = -1;
	}

	return rc;
}


int __stdcall ImSendData(int sess_hndl, char *json_str, char *resource_name)
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return -1;
  }

  int rc = im_send_data(pclient, json_str, resource_name);
  DBG("json_str=%s", json_str);
	if ( rc < 0  )	{
		ERR("fail im_send_data()\n");
		rc = -1;
	}

	return rc;
}

////////////////////////////////////////////////////
//
void __stdcall ImSet_strdata_handler(int sess_hndl,  char*(*func)(void *cli, char *devName, char *resource_name, char *prop_name, char *prop_value))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (문자형)
  ********************************************************/
  pclient->if525_handler_for_string = func;
}

void __stdcall ImSet_numdata_handler(int sess_hndl,  double(*func)(void *cli, char *devName, char *resource_name, char *prop_name, double prop_value))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (넘버형)
  ********************************************************/
  pclient->if525_handler_for_number = func;
  pclient->if525_handler_for_float = func;
}



void __stdcall ImSet_intdata_handler(int sess_hndl,  int(*func)(void *cli, char *devName, char *resource_name, char *prop_name, int prop_value))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (정수형)
  ********************************************************/
  pclient->if525_handler_for_integer = func;
}

void __stdcall ImSet_floatdata_handler(int sess_hndl,  double(*func)(void *cli, char *devName, char *resource_name, char *prop_name, double prop_value))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (실수형)
  ********************************************************/
  pclient->if525_handler_for_float = func;
}

void __stdcall ImSet_booldata_handler(int sess_hndl,  int(*func)(void *cli, char *devName, char *resource_name, char *prop_name, int prop_value))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (부울형(int))
  ********************************************************/
  pclient->if525_handler_for_boolean = func;
}


void __stdcall ImSet_endOfControl_handler(int sess_hndl,  int(*func)(void *cli, char *devName, char *resource_name, void *rep_body))
{
  im_client_tPtr pclient = sesspool_sess(sess_hndl);
  if ( pclient == NULL )  {
    return;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (부울형(int))
  ********************************************************/
  pclient->if525_handler_on_end_of_control = func;
}


#if 1 // 2020-11-16
int __stdcall ImSetReportDataOnDev(void *rep_body, char *json_str, char *resource_name, char *dev_id)
{
	return im_set_report_data_on_dev(rep_body, json_str, resource_name, dev_id);
}
#else
// will be deprecated
int __stdcall ImSetReportForControl(void *rep_body, char *resource_name, char *json_str)
{
	return im_if525_rep_set_data_in_json(rep_body, resource_name, json_str);
}
#endif




