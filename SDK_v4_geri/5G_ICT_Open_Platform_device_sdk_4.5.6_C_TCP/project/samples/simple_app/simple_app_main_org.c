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

#include <stdio.h>
#include "client/client.h"


static char g_report_str[250];

char* example_if525_request_handler_for_string(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%s]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	
	sprintf(g_report_str, "{\"%s\":\"%s\"}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_integer(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%d}", prop_name, prop_value);

	return prop_value;

}

double example_if525_request_handler_for_float(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, double prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%g]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%g}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_boolean(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%s}", prop_name, (prop_value==1)?"true":"false");

	return prop_value;
}

int example_if525_handler_on_end_of_control(im_client_tPtr cli, char *dev_id, char *resource_name, void *report_body)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s] End of control\n", dev_id, resource_name);
	printf("= g_report_str=[%s]\n", g_report_str);
	printf("====================================\n");
	// ?????? ???????????? ?????? ????????? ???????????????.
	// ??????????????? ????????? ?????? ????????????.

	// ????????? ??????Tag??? ???????????? ???????????? ??????
	if ( report_body != NULL )	{

		// Report in JSON String
		if ( strlen(g_report_str) > 0 )
		{
			im_set_report_data_on_dev(report_body, g_report_str, resource_name, dev_id);
			g_report_str[0] = '\0';
		} 
		else
		{
			char *json_str = "{\"command\":\"OFF\"}";
			im_set_report_data_on_dev(report_body, json_str, resource_name, dev_id);
		}
	}

	// ????????? ???????????? ????????? ??????
	return (100); // code: 100=OK, 200=ERR
}



/** 
 *  ??????????????? ?????? ???  
 */  
static int example_send_data_in_json_string(im_client_tPtr cli)
{
#if 1
  char *json_str = "{\"Temp\":25.5, \"status\":\"hello\"}";
#else
  char *json_str = "{"
					"\"testString\":\"hello\", "
					"\"testInteger\":9000, "
					"\"testDouble\":9.555, "
					"\"testBoolean\":true, "

					"\"testStringArray\":[\"aaa\", \"bbb\", \"ccc\", \"ddd\"], "
					"\"testIntegerArray\":[100, 200, 300], "
					"\"testDoubleArray\":[100.5, 200.5, 300.5], "
					"\"testBooleanArray\":[true,true,false,false]"
					"}";
#endif
  
	char *resource_name = "test";


#if 1	
	int rc = im_send_data(cli, json_str, resource_name);
#else
	/**********************************
	 *  ??????????????? ??????????????? ?????? ???  
	 ************************************/  
	char *dev_id = NULL;
	int rc = im_send_data_on_dev(cli, json_str, resource_name, dev_id);
	/************************************/  
#endif

	if ( rc < 0)  {
		ERR("fail im_send_data()");
		return -1;
	}

	printf("sent data\n");

	return 0;
}
//
//////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
  int rc = -1;

  /********************************************************
  *  IoTMakers ???????????? ??????????????? ?????????
  ********************************************************/
  im_client_t client;
  

  if ( argc != 6 )  {
    printf("Usage : %s {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}\n", argv[0]);
    exit(0);
  }

  char *ec_ip = argv[1];
  int ec_port = atoi(argv[2]);
  char *dev_id = argv[3];
  char *dev_pw = argv[4];
  char *svc_gw = argv[5];

  printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n", 
                        ec_ip, ec_port, dev_id, dev_pw, svc_gw);
  
  /********************************************************
  * ???????????? ??????
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(3);


  /********************************************************
  *  ????????? ?????????
  ********************************************************/
  rc = im_client_init(&client);
  if ( rc < 0 )  {
    return -1;
  }

  /********************************************************
  *  ???????????? ???????????? ????????? ?????? (?????????/?????????)
  ********************************************************/
  client.if525_handler_for_string = example_if525_request_handler_for_string;
  client.if525_handler_for_integer = example_if525_request_handler_for_integer;
  client.if525_handler_for_float  = example_if525_request_handler_for_float;
  client.if525_handler_for_boolean  = example_if525_request_handler_for_boolean;
  client.if525_handler_on_end_of_control  = example_if525_handler_on_end_of_control;

  /********************************************************
  *  ?????? ????????? ??? ???????????? ?????? ??????
  ********************************************************/
_AGAIN:

  /********************************************************
  *   EC ?????? ?????? ?????? (TCP ?????? ???????????????)
  ********************************************************/
  rc = im_connect_to(&client, ec_ip, ec_port);


  /********************************************************
  *  ???????????? ?????? ??????
  ********************************************************/
  rc = im_auth_device(&client, dev_id, dev_pw, svc_gw);
  if ( rc < 0 )  {
    im_disconnect(&client);
    return -1;
  }

  while ( 1==1 )  {

    /********************************************************
    *  ??????) ??????????????? ?????? 
    ********************************************************/
	rc = example_send_data_in_json_string(&client);
    if ( rc < 0 )  {
      // ??????????????? loop??????
      break;
    }

	im_msleep(1*1000);

    /********************************************************
    *  ????????? ????????? ??????
    ********************************************************/
    rc = im_poll(&client);
    if ( rc < 0 )  {
      // ????????? ?????? ????????? loop??????
      break;
    }

  }


  /********************************************************
  *  ?????? ??????
  ********************************************************/
  im_disconnect(&client);


  /********************************************************
  *  3??? ?????? ????????? ??????
  ********************************************************/
  //sleep(3);
  //goto _AGAIN;

  return 0;
}
