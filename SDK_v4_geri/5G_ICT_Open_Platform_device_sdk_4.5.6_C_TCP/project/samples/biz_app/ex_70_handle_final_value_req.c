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



static void example_put_lastvalue_in_json_onGroup_testAllType(void* res_body)
{
	char* resource_name = "testAllType";

	char *json_str = "{"
		"\"testString\":\"hello_last\", "
		"\"testInteger\":5000, "
		"\"testDouble\":90.555, "
		"\"testBoolean\":false, "

		"\"testStringArray\":[\"last_1\", \"last_2\", \"last_3\", \"last_4\"], "
		"\"testIntegerArray\":[1000, 2000, 3000], "
		"\"testDoubleArray\":[1000.5, 2000.5, 3000.5], "
		"\"testBooleanArray\":[true,true,false,false]"
		"}";

	im_if711_append_data_in_json(res_body, resource_name, json_str);
}

static void example_put_lastvalue_in_json_onGroup_testDummy(void* res_body)
{
	char* resource_name = "testDummy";

	char *json_str = "{"
		"\"testString\":\"hello_dummy\", "
		"\"testInteger\":0, "
		"\"testDouble\":0.0, "
		"\"testBoolean\":false"
		"}";

	im_if711_append_data_in_json(res_body, resource_name, json_str);
}

void example_if711_lastval_handler_for(im_client_tPtr cli, char *dev_id, char *resource_name, void* res_body)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s]\n", dev_id, resource_name);
	printf("====================================\n");

	/**
		디바이스에 등록된 resource_name(그룹태그)의 센싱태그의 마지막 값으로 응답
	*/

	if ( strcmp(resource_name, "testAllType") == 0 )	{
	
		example_put_lastvalue_in_json_onGroup_testAllType(res_body);

	} else if ( strcmp(resource_name, "testDummy") == 0 )	{
	
		example_put_lastvalue_in_json_onGroup_testDummy(res_body);

	} else { 
		printf("====================================\n");
		printf("= Unregistered resource_name:[%s]\n", resource_name);
		printf("====================================\n");
	}
}



void example_if711_lastval_handler_for_all(im_client_tPtr cli, char *dev_id, void* res_body)
{
	printf("====================================\n");
	printf("= devid=[%s]\n", dev_id);
	printf("====================================\n");

	example_put_lastvalue_in_json_onGroup_testAllType(res_body);
	example_put_lastvalue_in_json_onGroup_testDummy(res_body);

}



///////////////////////////////////////////////////////////
//
#if 0
static void example_put_lastvalue_in_api(void* res_body)
{
	char *resource_name = "testGroupCode";


	// append new colecRow with resource_name
	im_body711_res_append_colecRow(res_body, resource_name);



	im_body711_res_append_string_data(res_body, "testString", "hello_last");

	im_body711_res_append_int_data(res_body, "testInteger", (int)555);
	
	im_body711_res_append_realnum_data(res_body, "testDouble", (double)10.555);
	
	im_body711_res_append_bool_data(res_body, "testBoolean", (int)1);


	/* 리스트 타입의 데이터 */

	char  *el_str_arr[10];
	double el_arr[10];
	int    el_cnt = 0;

	el_str_arr[0] = "last_hello_1";
	el_str_arr[1] = "last_hello_2";
	el_str_arr[2] = "last_hello_3";
	el_cnt = 3;
	im_body711_res_append_string_data_list(res_body, "testStringArray", el_cnt, el_str_arr) ;


	el_arr[0] = (int)111;
	el_arr[1] = (int)222;
	el_arr[2] = (int)333;
	el_cnt = 3;
	im_body711_res_append_int_data_list(res_body, "testIntegerArray", el_cnt, el_arr) ;
	

	el_arr[0] = (double)10000.555;
	el_arr[1] = (double)20000.555;
	el_arr[2] = (double)30000.555;
	el_cnt = 3;
	im_body711_res_append_realnum_data_list(res_body, "testDoubleArray", el_cnt,el_arr) ;
	

	el_arr[0] = (int)1;
	el_arr[1] = (int)1;
	el_arr[2] = (int)0;
	el_cnt = 3;
	im_body711_res_append_bool_data_list(res_body, "testBooleanArray", el_cnt, el_arr) ;
}
#endif


