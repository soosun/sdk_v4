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
#include "kt_iot_log.h"
#include "client/client.h"

static char g_report_str[250];

char* example_if525_request_handler_for_string(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%s]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	// 여기 디바이스 제어 크드를 작성하세요.
	// 제어로직은 최대한 빨리 끝내세요.
	
	sprintf(g_report_str, "{\"%s\":\"%s\"}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_integer(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	// 여기 디바이스 제어 크드를 작성하세요.
	// 제어로직은 최대한 빨리 끝내세요.

	sprintf(g_report_str, "{\"%s\":%d}", prop_name, prop_value);


	return prop_value;
}

double example_if525_request_handler_for_float(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, double prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%g]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	// 여기 디바이스 제어 크드를 작성하세요.
	// 제어로직은 최대한 빨리 끝내세요.

	sprintf(g_report_str, "{\"%s\":%g}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_boolean(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	// 여기 디바이스 제어 크드를 작성하세요.
	// 제어로직은 최대한 빨리 끝내세요.

	sprintf(g_report_str, "{\"%s\":%s}", prop_name, (prop_value==1)?"true":"false");

	return prop_value;
}

int example_if525_handler_on_end_of_control(im_client_tPtr cli, char *dev_id, char *resource_name, void *report_body)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s] End of control\n", dev_id, resource_name);
	printf("====================================\n");

	// 여기 디바이스 제어 크드를 작성하세요.
	// 제어로직은 최대한 빨리 끝내세요.

	// 응답용 제어Tag별 결과값을 세팅하는 예제
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

	// 응답용 제어결과 코드를 리턴
	return (100); // code: 100=OK, 200=ERR
}
