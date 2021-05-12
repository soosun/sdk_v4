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
#include <stdarg.h>

#include "kt_iot_log.h"
#include "util/util.h"
#include "client/client.h"


///////////////////////////////////////////////////////////////////
/** 411 수집전송 수신처리 */
///////////////////////////////////////////////////////////////////
static void response_packet_handler_411(im_client_tPtr cli, IMPacketPtr res)
{
  unsigned long long trx_id = im_head_get_trmTransacId(&res->head) ;
  INF("respCd[%s]", im_body411_res_get_respCd(&res->body));
}

static void circuit_breaker(im_client_tPtr cli)
{
  if ( cli->circuit_breaker_sw == 0 )  {
    return;
  }

  if ( IOTMAKERS_CIRCUIT_BREAKER_IN_SEC > 0 )
  {
    while ( (im_util_gettimeofday_as_sec() - cli->circuit_breaker_timestamp) <  IOTMAKERS_CIRCUIT_BREAKER_IN_SEC )
    {
      WRN("IOTMAKERS_CIRCUIT_BREAKER_IN_SEC=[%d secs]", IOTMAKERS_CIRCUIT_BREAKER_IN_SEC);
      im_msleep(1*1000);
    }
    cli->circuit_breaker_timestamp = im_util_gettimeofday_as_sec(); 
  }
}


static int set_default_body411(im_client_tPtr cli, IMPacketPtr req)
{
  // body 초기화 (Service G/W)
  im_body411_req_set_extrSysId(&req->body, cli->svc_gw);

  // body->디바이스셋 추가 (Device ID)
  im_body411_req_append_devColecData(&req->body, cli->dev_id);
  
  // body->디바이스셋->레코드셋 추가 (그룹태그 옵션)
  im_body411_req_append_colecRow(&req->body, NULL, NULL);
  return 0;
}


/*
// count 쌍의 double value paire 전송 (char *tag_name, double tag_value)

int rc = im_send_number_data_complex(cli, 2
  "tag1", (double)100,
  "tag2", (double)110 
  )

*/
int im_send_number_data_complex(im_client_tPtr cli, int count, ...)
{
  IMPacket req;
  va_list   arg;
  int i;

  circuit_breaker(cli);


  //(장비인증토큰)
  int rc = im_packet411_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  set_default_body411(cli, &req);

	va_start( arg, count*2);
	for (i = 0; i < count; i++)	{
    // body->디바이스셋->레코드셋->숫자형 Tag추가
    char *tag_name = va_arg( arg, char*);
    double tag_value = va_arg( arg, double);
    im_body411_req_set_number_data(&req.body, tag_name, tag_value);
	}
	va_end( arg);

    if ( cli->response_wait_sw == (1) )
    {
        rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_411);
    } else {
        // 2020-04-16 : 응답미대기
        rc = im_transact_packet_with_response_handler(cli, &req, NULL);
    }

  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  return 0;
}



/*
// count 쌍의 char* value paire 전송 (char *tag_name, double tag_value)

int rc = im_send_string_data_complex(cli, 2
  "tag1", "hello",
  "tag2", "world"
  )

*/
int im_send_string_data_complex(im_client_tPtr cli, int count, ...)
{
  IMPacket req;
  va_list   arg;
  int i;

  circuit_breaker(cli);


  //im_packet411_req_init (장비인증토큰)
  int rc = im_packet411_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  set_default_body411(cli, &req);

	va_start( arg, count*2);
	for (i = 0; i < count; i++)	{
    // body->디바이스셋->레코드셋->숫자형 Tag추가 ()
    char *tag_name = va_arg( arg, char*);
    char *tag_value = va_arg( arg, char*);
    im_body411_req_set_string_data(&req.body, tag_name, tag_value);
	}
	va_end( arg);

    if ( cli->response_wait_sw == (1) )
    {
        rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_411);
    } else {
        // 2020-04-16 : 응답미대기
        rc = im_transact_packet_with_response_handler(cli, &req, NULL);
    }




  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  return 0;
}


int im_send_number_data(im_client_tPtr cli, char *tag_name, double tag_value)
{
  return im_send_number_data_complex(cli, 1, tag_name, tag_value);
}


int im_send_string_data(im_client_tPtr cli, char *tag_name, char *tag_value)
{
  return im_send_string_data_complex(cli, 1, tag_name, tag_value);
}



static int __is_number_integer(double number)
{
	char buff[64];
	sprintf(buff, "%g", number);
	if ( strchr(buff, '.') == NULL )		{
		return (1);
	}
	return 0;
}

static int __is_string_datetime(char* str)
{
	/*
	"2020-10-08 12:00:00.000"
	*/
	if ( strlen(str) != 23 )	{
		return 0;
	}

	if ( str[4]!='-' || str[7]!='-' || str[13]!=':' || str[16]!=':' || str[19]!='.')	{
		return 0;
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
/*
2020-04-01
added grouptag
*/
int im_send_data_in_parson(im_client_tPtr cli, JSON_Value *j_val, char *resource_name, char *dev_id)
{
  int i;
  IMPacket req;


  circuit_breaker(cli);

  
  if ( j_val == NULL )  {
    ERR("j_val is NULL");
    return -1;
  }

  JSON_Object *j_obj = json_object (j_val);
  if ( j_obj == NULL )  {
    ERR("j_obj is NULL");
    return -1;
  }

  size_t j_cnt = json_object_get_count(j_obj);
  if ( j_cnt == 0 )  {
    ERR("j_cnt is 0");
    return -1;
  }


  //im_packet411_req_init (장비인증토큰)
  int rc = im_packet411_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  if ( dev_id == NULL &&  resource_name == NULL)  {
    set_default_body411(cli, &req);
  } else {
    im_body411_req_set_extrSysId(&req.body, cli->svc_gw);

      if ( dev_id != NULL )  {
        im_body411_req_append_devColecData(&req.body, dev_id);
      } else {
        im_body411_req_append_devColecData(&req.body, cli->dev_id);
      }

      if ( resource_name != NULL )  {
        im_body411_req_append_colecRow(&req.body, NULL, resource_name);
      } else {
        im_body411_req_append_colecRow(&req.body, NULL, NULL);
      }
  }
   

  
  for (i = 0; i < j_cnt; i++)  {

    const char *name = json_object_get_name(j_obj, i);
    
	JSON_Value *j_val_i = json_object_get_value(j_obj, name);
    
	if ( j_val_i == NULL )  {
      ERR("j_val_i is NULL");
      im_packet_release(&req);
      return -1;
    }

	/*
	enum json_value_type {
		JSONError   = -1,
		JSONNull    = 1,
		JSONString  = 2,
		JSONNumber  = 3,
		JSONObject  = 4,
		JSONArray   = 5,
		JSONBoolean = 6
	};
	*/
    JSON_Value_Type type = json_value_get_type (j_val_i);

	switch ( type )
    {
    case JSONString: {
      const char *s_val = json_string(j_val_i);
		if ( __is_string_datetime(s_val) == (1) )		{
			im_body411_req_set_datetimestr_data(&req.body, name, s_val);
		} else {
			im_body411_req_set_string_data(&req.body, name, s_val);
		}
    }
      break;  

    case JSONNumber: {
      double n_val = json_number(j_val_i);
		if ( __is_number_integer(n_val) == (1) )		{
			int int_val = (int)n_val;
			im_body411_req_set_int_data(&req.body, name, int_val);
		} else {
			im_body411_req_set_realnum_data(&req.body, name, n_val);
		}
    }
	break;  

    case JSONObject: {
      char *buff = json_serialize_to_string(j_val_i);
      im_body411_req_set_string_data(&req.body, name, buff);
      json_free_serialized_string(buff);
    }
      break;  

    case JSONNull: {
      DBG("[%s] in [JSONNull]", name);
    }
      break;  

    case JSONBoolean: {
		int boolVal = json_boolean(j_val_i);
      im_body411_req_set_bool_data(&req.body, name, boolVal);
    }
      break;  

    case JSONArray: {
		JSON_Array  *j_arr = json_array  (j_val_i);
		int el_cnt = (int)json_array_get_count(j_arr);
		
		char *el_str_arr[100];
		double el_arr[100];

		JSON_Value_Type type_el = -1;

		double sample_number = 0.0;
		char* sample_str = NULL;

		for (i=0; i<el_cnt; i++)
		{
			JSON_Value *j_val_el = json_array_get_value(j_arr, i);
			type_el = json_value_get_type (j_val_el);

			if ( type_el ==  JSONString )
			{
				const char *s_val = json_string(j_val_el);
				el_str_arr[i] = s_val;
				sample_str = s_val;
			} 
			else if ( type_el ==  JSONNumber )
			{
				double n_val = json_number(j_val_el);

				el_arr[i] = n_val;
				sample_number = n_val;
			}
			else if ( type_el ==  JSONBoolean )
			{
				int boolVal = json_boolean(j_val_el);
				el_arr[i] = (double)boolVal;
			}
			else {
				DBG("JSON_Value_Type is [%d]", type_el);
			}
		}

		if ( type_el ==  JSONString )
		{
			if ( __is_string_datetime(sample_str) == (1) )		{
				im_body411_req_set_string_datetimestr_list(&req.body, name, el_cnt, el_str_arr);
			} else {
				im_body411_req_set_string_data_list(&req.body, name, el_cnt, el_str_arr);
			}
		} 
		else if ( type_el ==  JSONNumber )
		{
			if ( __is_number_integer(sample_number) == (1) )		{
				im_body411_req_set_int_data_list(&req.body, name, el_cnt, el_arr);
			} else {
				im_body411_req_set_realnum_data_list(&req.body, name, el_cnt, el_arr);
			}
		}
		else if ( type_el ==  JSONBoolean )
		{
	      im_body411_req_set_bool_data_list(&req.body, name, el_cnt, el_arr);
		}
    }
      break;  

    default:
      WRN("j_type [%d] not supprted", type);
    }
  }

    if ( cli->response_wait_sw == (1) )
    {
          rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_411);
    } else {
        // 2020-04-16 : 응답미대기
        rc = im_transact_packet_with_response_handler(cli, &req, NULL);
    }


    if ( rc < 0 )    {
        ERR("fail im_transact_packet_with_response_handler()");
        im_packet_release(&req);
        return -1;
      }

  im_packet_release(&req);
  return 0;
}



int im_send_data_on_dev(im_client_tPtr cli, char *json_str, char *resource_name, char *dev_id)
{
  JSON_Value *j_val = json_parse_string_with_comments(json_str);
  if ( j_val == NULL )  {
    ERR("j_val is NULL; json_str=%s", json_str);
    return -1;
  }

  int rc = im_send_data_in_parson(cli, j_val, resource_name, dev_id);

  json_value_free(j_val);

  return rc;
}

#if 0
int im_send_data_in_json_str_to_dev(im_client_tPtr cli, char *json_str, char *dev_id)
{
  return im_send_data_on_dev(cli, json_str, NULL, dev_id);
}
#endif

int im_send_data(im_client_tPtr cli, char *json_str, char *resource_name)
{
  return im_send_data_on_dev(cli, json_str, resource_name, NULL);
}







///////////////////////////////////////////////////////////////////
/** 525 제어수신 */
///////////////////////////////////////////////////////////////////
#if 1
int im_set_report_data_on_dev(IMPacketBodyPtr body, char *json_str, char *resource_name, char *dev_id)
{
  int i;

  JSON_Value *j_val = json_parse_string_with_comments(json_str);
  if ( j_val == NULL )  {
    ERR("j_val is NULL; json_str=%s", json_str);
    return -1;
  }

  
  if ( j_val == NULL )  {
    ERR("j_val is NULL");
    return -1;
  }

  JSON_Object *j_obj = json_object (j_val);
  if ( j_obj == NULL )  {
    ERR("j_obj is NULL");
    json_value_free(j_val);
    return -1;
  }

  size_t j_cnt = json_object_get_count(j_obj);
  if ( j_cnt == 0 )  {
    ERR("j_cnt is 0");
    json_value_free(j_val);
    return -1;
  }

  im_body525_rep_append_devCnvyData(body, dev_id);

  im_body525_rep_append_cnvyRow(body, NULL, resource_name); 

  for (i = 0; i < j_cnt; i++)  {

    const char *name = json_object_get_name(j_obj, i);
    
	JSON_Value *j_val_i = json_object_get_value(j_obj, name);
    
	if ( j_val_i == NULL )  {
      ERR("j_val_i is NULL");
	  json_value_free(j_val);
      return -1;
    }

	/*
	enum json_value_type {
		JSONError   = -1,
		JSONNull    = 1,
		JSONString  = 2,
		JSONNumber  = 3,
		JSONObject  = 4,
		JSONArray   = 5,
		JSONBoolean = 6
	};
	*/
    JSON_Value_Type type = json_value_get_type (j_val_i);
    switch ( type )
    {
    case JSONString: {
      const char *s_val = json_string(j_val_i);
		if ( __is_string_datetime(s_val) )		{
			/*
			im_body711_rep_set_datetimestr_data(body, name, s_val);
			*/
			WRN("N/A im_body711_rep_set_datetimestr_data()");
		} else {
			//im_body711_res_append_string_data(body, name, s_val);
			im_body525_rep_set_string_data(body, name, s_val);
		}
    }
      break;  

    case JSONNumber: {
      double n_val = json_number(j_val_i);
		if ( __is_number_integer(n_val) )		{
			im_body525_rep_set_int_data(body, name, (int)n_val);
		} else {
			im_body525_rep_set_realnum_data(body, name, n_val);
		}
    }
      break;  

    case JSONObject: {
      char *buff = json_serialize_to_string(j_val_i);
      im_body525_rep_set_string_data(body, name, buff);
      json_free_serialized_string(buff);
    }
      break;  

    case JSONNull: {
      DBG("[%s] in [JSONNull]", name);
    }
      break;  

    case JSONBoolean: {
		int boolVal = json_boolean(j_val_i);
      im_body525_rep_set_bool_data(body, name, boolVal);
    }
      break;  

    case JSONArray: {
		JSON_Array  *j_arr = json_array  (j_val_i);
		int el_cnt = (int)json_array_get_count(j_arr);
		
		char *el_str_arr[100];
		double el_arr[100];

		JSON_Value_Type type_el = -1;

		double sample_number = 0.0;
		char* sample_str = NULL;

		for ( i=0; i<el_cnt; i++)
		{
			JSON_Value *j_val_el = json_array_get_value(j_arr, i);
			type_el = json_value_get_type (j_val_el);

			if ( type_el ==  JSONString )
			{
				const char *s_val = json_string(j_val_el);
				el_str_arr[i] = s_val;
				sample_str = s_val;
			} 
			else if ( type_el ==  JSONNumber )
			{
				double n_val = json_number(j_val_el);

				el_arr[i] = n_val;
				sample_number = n_val;
			}
			else if ( type_el ==  JSONBoolean )
			{
				int boolVal = json_boolean(j_val_el);
				el_arr[i] = (double)boolVal;
			}
			else {
				DBG("JSON_Value_Type is [%d]", type_el);
			}
		}

		if ( type_el ==  JSONString )
		{
			if ( __is_string_datetime(sample_str) )		{
				//im_body525_rep_set_string_datetimestr_list(body, name, el_cnt, el_str_arr);
				WRN("N/A im_body525_rep_set_string_datetimestr_list()");
			} else {
				//im_body525_rep_set_string_data_list(body, name, el_cnt, el_str_arr);
				WRN("N/A im_body525_rep_set_string_data_list()");
			}
		}
		else if ( type_el ==  JSONNumber )
		{
			if ( __is_number_integer(sample_number) )		{
				//im_body525_rep_set_int_data_list(body, name, el_cnt, el_arr);
				WRN("N/A im_body525_rep_set_int_data_list()");
			} else {
				//im_body525_rep_set_realnum_data_list(body, name, el_cnt, el_arr);
				WRN("N/A im_body525_rep_set_realnum_data_list()");
			}
		}
		else if ( type_el ==  JSONBoolean )
		{
	      //im_body525_rep_set_bool_data_list(body, name, el_cnt, el_arr);
		  WRN("N/A im_body525_rep_set_bool_data_list()");
		}
    }
      break;  

    default:
      WRN("j_type [%d] not supprted", type);
    }
  }


  json_value_free(j_val);

  return 0;
}

#else
int im_if525_rep_set_data_in_json(IMPacketBodyPtr body, char *resource_name, char *json_str)
{
  int i;

  JSON_Value *j_val = json_parse_string_with_comments(json_str);
  if ( j_val == NULL )  {
    ERR("j_val is NULL; json_str=%s", json_str);
    return -1;
  }

  
  if ( j_val == NULL )  {
    ERR("j_val is NULL");
    return -1;
  }

  JSON_Object *j_obj = json_object (j_val);
  if ( j_obj == NULL )  {
    ERR("j_obj is NULL");
    json_value_free(j_val);
    return -1;
  }

  size_t j_cnt = json_object_get_count(j_obj);
  if ( j_cnt == 0 )  {
    ERR("j_cnt is 0");
    json_value_free(j_val);
    return -1;
  }

  //
  im_body525_rep_append_devCnvyData(body, cli->dev_id);


  // append new colecRow with resource_name
  im_body525_rep_append_cnvyRow(body, NULL, resource_name); 


  for (i = 0; i < j_cnt; i++)  {

    const char *name = json_object_get_name(j_obj, i);
    
	JSON_Value *j_val_i = json_object_get_value(j_obj, name);
    
	if ( j_val_i == NULL )  {
      ERR("j_val_i is NULL");
	  json_value_free(j_val);
      return -1;
    }

	/*
	enum json_value_type {
		JSONError   = -1,
		JSONNull    = 1,
		JSONString  = 2,
		JSONNumber  = 3,
		JSONObject  = 4,
		JSONArray   = 5,
		JSONBoolean = 6
	};
	*/
    JSON_Value_Type type = json_value_get_type (j_val_i);
    switch ( type )
    {
    case JSONString: {
      const char *s_val = json_string(j_val_i);
		if ( __is_string_datetime(s_val) )		{
			/*
			im_body711_rep_set_datetimestr_data(body, name, s_val);
			*/
			WRN("N/A im_body711_rep_set_datetimestr_data()");
		} else {
			//im_body711_res_append_string_data(body, name, s_val);
			im_body525_rep_set_string_data(body, name, s_val);
		}
    }
      break;  

    case JSONNumber: {
      double n_val = json_number(j_val_i);
		if ( __is_number_integer(n_val) )		{
			im_body525_rep_set_int_data(body, name, (int)n_val);
		} else {
			im_body525_rep_set_realnum_data(body, name, n_val);
		}
    }
      break;  

    case JSONObject: {
      char *buff = json_serialize_to_string(j_val_i);
      im_body525_rep_set_string_data(body, name, buff);
      json_free_serialized_string(buff);
    }
      break;  

    case JSONNull: {
      DBG("[%s] in [JSONNull]", name);
    }
      break;  

    case JSONBoolean: {
		int boolVal = json_boolean(j_val_i);
      im_body525_rep_set_bool_data(body, name, boolVal);
    }
      break;  

    case JSONArray: {
		JSON_Array  *j_arr = json_array  (j_val_i);
		int el_cnt = (int)json_array_get_count(j_arr);
		
		char *el_str_arr[100];
		double el_arr[100];

		JSON_Value_Type type_el = -1;

		double sample_number = 0.0;
		char* sample_str = NULL;

		for ( i=0; i<el_cnt; i++)
		{
			JSON_Value *j_val_el = json_array_get_value(j_arr, i);
			type_el = json_value_get_type (j_val_el);

			if ( type_el ==  JSONString )
			{
				const char *s_val = json_string(j_val_el);
				el_str_arr[i] = s_val;
				sample_str = s_val;
			} 
			else if ( type_el ==  JSONNumber )
			{
				double n_val = json_number(j_val_el);

				el_arr[i] = n_val;
				sample_number = n_val;
			}
			else if ( type_el ==  JSONBoolean )
			{
				int boolVal = json_boolean(j_val_el);
				el_arr[i] = (double)boolVal;
			}
			else {
				DBG("JSON_Value_Type is [%d]", type_el);
			}
		}

		if ( type_el ==  JSONString )
		{
			if ( __is_string_datetime(sample_str) )		{
				//im_body525_rep_set_string_datetimestr_list(body, name, el_cnt, el_str_arr);
				WRN("N/A im_body525_rep_set_string_datetimestr_list()");
			} else {
				//im_body525_rep_set_string_data_list(body, name, el_cnt, el_str_arr);
				WRN("N/A im_body525_rep_set_string_data_list()");
			}
		}
		else if ( type_el ==  JSONNumber )
		{
			if ( __is_number_integer(sample_number) )		{
				//im_body525_rep_set_int_data_list(body, name, el_cnt, el_arr);
				WRN("N/A im_body525_rep_set_int_data_list()");
			} else {
				//im_body525_rep_set_realnum_data_list(body, name, el_cnt, el_arr);
				WRN("N/A im_body525_rep_set_realnum_data_list()");
			}
		}
		else if ( type_el ==  JSONBoolean )
		{
	      //im_body525_rep_set_bool_data_list(body, name, el_cnt, el_arr);
		  WRN("N/A im_body525_rep_set_bool_data_list()");
		}
    }
      break;  

    default:
      WRN("j_type [%d] not supprted", type);
    }
  }


  json_value_free(j_val);

  return 0;
}
#endif




static int __if525_process_req_cnvyRow(im_client_tPtr cli, IMPacketPtr pkt, char *resource_name, IMPacketPtr reportPkt)
{
	int rc = 0;
	char *devid;
	char *prop_name;

	double numval;
	char *strval;
	double floatval;
	int intval;
	int boolval;
	
	double numval_ret;
	char *strval_ret;
	double floatval_ret;
	int intval_ret;
	int boolval_ret;

	int idx = 0;
	int happens = 0;
	int retCode4Control = (-1);
	
	cli->failControlCnt = 0;

	// WILL BE DEPRECATED, 2020-10-13
    // 숫자형 데이터 처리
	if ( cli->if525_handler_for_number != NULL )	
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)	{
			rc = im_body525_req_get_numdata_with_index(&pkt->body, idx, &devid, &prop_name, &numval);
			if ( rc < 0 )	{
				break;
			}
		  /////////////////////////
		  // 사용자 정의 함수 콜백
		  /////////////////////////
		  numval_ret = cli->if525_handler_for_number(cli, devid, resource_name, prop_name, numval);
#if 0
		  DBG("ControlResult [%s]:[%g]->[%g]", prop_name, numval, numval_ret);
			
			if ( numval != numval_ret )			{
				cli->failControlCnt++;
			}

			if ( reportPkt != NULL )			{
				// set Control Report Value
				DBG("Adding Control Report Val:[%g]", numval_ret);
				im_body525_rep_set_number_data(&reportPkt->body, prop_name, numval_ret);
			}
#endif
	      happens++;
		}
	} else {
		;
		//WRN("No control_numdata_handler set");
	}


	// 문자형 제어 데이터 처리
	if (cli->if525_handler_for_string != NULL)
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)		{
			rc = im_body525_req_get_strdata_with_index(&pkt->body, idx, &devid, &prop_name, &strval);
			if ( rc < 0 )	{
				break;
			}

		    // 사용자 정의 함수 콜백
			strval_ret = cli->if525_handler_for_string(cli, devid, resource_name, prop_name, strval);	
#if 0
		  DBG("ControlResult [%s]:[%s]->[%s]", prop_name, strval, strval_ret);

			if ( strcmp(strval, strval_ret) != 0 )			{
				cli->failControlCnt++;
			}

			if ( reportPkt != NULL )			{
				// set Control Report Value
				DBG("Adding Control Report Val:[%s]", strval_ret);
				im_body525_rep_set_string_data(&reportPkt->body, prop_name, strval_ret);
			}
#endif
			happens++;
		}
	} else {
		;
		//WRN("No if525_handler_for_string set");
	}


	// 정수형 제어 데이터 처리
	if (cli->if525_handler_for_integer != NULL)
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)		{
			rc = im_body525_req_get_integerdata_with_index(&pkt->body, idx, &devid, &prop_name, &intval);
			if ( rc < 0 )	{
				break;
			}

		    // 사용자 정의 함수 콜백
			intval_ret = cli->if525_handler_for_integer(cli, devid, resource_name, prop_name, intval);	
#if 0
		  DBG("ControlResult [%s]:[%d]->[%d]", prop_name, intval, intval_ret);

			if ( intval != intval_ret )			{
				cli->failControlCnt++;
			}

			if ( reportPkt != NULL )			{
				// set Control Report Value
				DBG("Adding Control Report Val:[%d]", intval_ret);
				im_body525_rep_set_int_data(&reportPkt->body, prop_name, intval_ret);
			}
#endif
			happens++;
		}
	} else {
		;
		//WRN("No if525_handler_for_integer set");
	}


	// 실수형 제어 데이터 처리
	if (cli->if525_handler_for_float != NULL)
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)		{
			rc = im_body525_req_get_floatdata_with_index(&pkt->body, idx, &devid, &prop_name, &floatval);
			if ( rc < 0 )	{
				break;
			}

		    // 사용자 정의 함수 콜백
			floatval_ret = cli->if525_handler_for_float(cli, devid, resource_name, prop_name, floatval);	
#if 0
		  DBG("ControlResult [%s]:[%g]->[%g]", prop_name, floatval, floatval_ret);

			if ( floatval != floatval_ret )			{
				cli->failControlCnt++;
			}

			if ( reportPkt != NULL )			{
				// set Control Report Value
				DBG("Adding Control Report Val:[%g]", floatval_ret);
				im_body525_rep_set_realnum_data(&reportPkt->body, prop_name, floatval_ret);
			}
#endif
			happens++;
		}
	} else {
		;
		//WRN("No if525_handler_for_float set");
	}


	// 부울형 제어 데이터 처리
	if (cli->if525_handler_for_boolean != NULL)
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)		{
			rc = im_body525_req_get_booleandata_with_index(&pkt->body, idx, &devid, &prop_name, &boolval);
			if ( rc < 0 )	{
				break;
			}

		    // 사용자 정의 함수 콜백
			boolval_ret = cli->if525_handler_for_boolean(cli, devid, resource_name, prop_name, boolval);	
#if 0
		  DBG("ControlResult [%s]:[%d]->[%d]", prop_name, boolval, boolval_ret);

			if ( boolval != boolval_ret )			{
				cli->failControlCnt++;
			}

			if ( reportPkt != NULL )			{
				// set Control Report Value
				DBG("Adding Control Report Val:[%d]", boolval_ret);
				im_body525_rep_set_bool_data(&reportPkt->body, prop_name, boolval_ret);
			}
#endif
			happens++;
		}
	} else {
		;
		//WRN("No if525_handler_for_boolean set");
	}


	// End of control
	if (cli->if525_handler_on_end_of_control != NULL && happens > 0 )
	{
		if ( reportPkt == NULL )			{
			// 사용자 정의 함수 콜백
			retCode4Control = cli->if525_handler_on_end_of_control(cli, devid, resource_name, NULL);	
		} 
		else
		{
			// 사용자 정의 함수 콜백
			retCode4Control = cli->if525_handler_on_end_of_control(cli, devid, resource_name, &reportPkt->body);	

			// send report
			DBG("retCode4Control; RespCd=[%d]", retCode4Control);
			if ( retCode4Control <= (0) )		{
				im_head_set_respCd(&reportPkt->head, RespCd_NORMALITY); //100
			} else {
				im_head_set_respCd(&reportPkt->head, retCode4Control); //100
			};

			return happens;
		}
	}

	
	if ( happens > 0 && reportPkt != NULL )
	{
		// send report
		if ( cli->failControlCnt == 0 )	{
			DBG("Control OK; failCnt[%d];RespCd=[%d]", cli->failControlCnt, RespCd_NORMALITY);
			im_head_set_respCd(&reportPkt->head, RespCd_NORMALITY); // 100
		} else {
			DBG("Control FAIL; failCnt[%d];RespCd=[%d]", cli->failControlCnt, RespCd_GENERAL_ERROR);
			im_head_set_respCd(&reportPkt->head, RespCd_GENERAL_ERROR); // 200
		}
	}


	return happens;
}

static int __if525_process_req_devCnvyData(im_client_tPtr cli, IMPacketPtr pkt, char *resource_name, IMPacketPtr reportPkt)
{
  int happens = 0;
  while ( __if525_process_req_cnvyRow(cli, pkt, resource_name, reportPkt) > 0 )    {
    im_body525_req_next_colecRowIdx(&pkt->body);
    happens++;
  }
  return happens;
}


void im_packet525_request_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
    // 데이터 핸들링을 위한 초기화
    im_body525_req_start_parse(&req->body);

	// GroupTagName
	char *resource_name = NULL;
	im_body525_req_get_groupTagCode(&req->body, &resource_name);
	if ( resource_name == NULL )	{
		DBG("resource_name is NULL");
	} else {
		DBG("resource_name=[%s]", resource_name);
	}

	// TransacId
	unsigned long long trx_id = im_head_get_trmTransacId(&req->head);

	/////////////////////////////////////////
	// 1. set Ready ReportPacket
	/////////////////////////////////////////
	IMPacket rep;
	int rc = im_packet525_rep_init(&rep, cli->commChAthnNo, trx_id);
	if ( rc < 0 )    {
		ERR("====================================");
		ERR("FAIL im_packet525_rep_init()");
		ERR("====================================");
		// 레포트 송신 없어 제어처리 시도
		while ( __if525_process_req_devCnvyData(cli, req, resource_name, NULL) > 0 )    {
		  im_body525_req_next_devColecDataIdx(&req->body);
		};
		return;
	}
	
	im_body525_rep_set_extrSysId(&rep.body, cli->svc_gw);

	// will be removed 2020-11-16
	// im_body525_rep_append_devCnvyData(&rep.body, cli->dev_id);
		
	// will be removed 2020-10-30
	// im_body525_rep_append_cnvyRow(&rep.body, NULL, resource_name); 
	//

	/////////////////////////////////////////
	// 2. 제어처리 
	/////////////////////////////////////////
    while ( __if525_process_req_devCnvyData(cli, req, resource_name, &rep) > 0 )    {
      im_body525_req_next_devColecDataIdx(&req->body);
    }

	/////////////////////////////////////////
	// 3. 제어응답 전송(Report)
	//__if525_send_report(cli, resource_name, trx_id);
	/////////////////////////////////////////
	rc = im_transact_packet_with_response_handler(cli, &rep, NULL);
	if ( rc < 0 )    {
	  ERR("fail im_transact_packet_with_response_handler()");
	  im_packet_release(&rep);
	  return;
	}

	/////////////////////////////////////////
	// 4. release Report Packet
	/////////////////////////////////////////
	im_packet_release(&rep);

	return;
}









/////////////////////////////////////////////////////////////////////////////////////////////////////
// 최종값 조회
/////////////////////////////////////////////////////////////////////////////////////////////////////
static void __sample_if711_handler_for_device_n_tag(im_client_tPtr cli, IMPacketPtr res, char *devId, char *prop_name)
{
#if 0	// if value of prop_name is string
	char *sample_string = "OK";
	im_body711_res_append_string_data(&res->body, prop_name, sample_string);
#else   // if value of prop_name is number
	double sample_number = 999.0;
	im_body711_res_append_realnum_data(&res->body, prop_name, sample_number);
#endif
}



int im_if711_append_data_in_json(IMPacketBodyPtr body, char *resource_name, char *json_str)
{
  int i;

  JSON_Value *j_val = json_parse_string_with_comments(json_str);
  if ( j_val == NULL )  {
    ERR("j_val is NULL; json_str=%s", json_str);
    return -1;
  }

  
  if ( j_val == NULL )  {
    ERR("j_val is NULL");
    return -1;
  }

  JSON_Object *j_obj = json_object (j_val);
  if ( j_obj == NULL )  {
    ERR("j_obj is NULL");
    json_value_free(j_val);
    return -1;
  }

  size_t j_cnt = json_object_get_count(j_obj);
  if ( j_cnt == 0 )  {
    ERR("j_cnt is 0");
    json_value_free(j_val);
    return -1;
  }


  // append new colecRow with resource_name
  im_body711_res_append_colecRow(body, resource_name);


  for (i = 0; i < j_cnt; i++)  {

    const char *name = json_object_get_name(j_obj, i);
    
	JSON_Value *j_val_i = json_object_get_value(j_obj, name);
    
	if ( j_val_i == NULL )  {
      ERR("j_val_i is NULL");
	  json_value_free(j_val);
      return -1;
    }

	/*
	enum json_value_type {
		JSONError   = -1,
		JSONNull    = 1,
		JSONString  = 2,
		JSONNumber  = 3,
		JSONObject  = 4,
		JSONArray   = 5,
		JSONBoolean = 6
	};
	*/
    JSON_Value_Type type = json_value_get_type (j_val_i);
    switch ( type )
    {
    case JSONString: {
      const char *s_val = json_string(j_val_i);
		if ( __is_string_datetime(s_val) )		{
			im_body711_res_append_datetimestr_data(body, name, s_val);
		} else {
			im_body711_res_append_string_data(body, name, s_val);
		}
    }
      break;  

    case JSONNumber: {
      double n_val = json_number(j_val_i);
		if ( __is_number_integer(n_val) )		{
			im_body711_res_append_int_data(body, name, (int)n_val);
		} else {
			im_body711_res_append_realnum_data(body, name, n_val);
		}
    }
      break;  

    case JSONObject: {
      char *buff = json_serialize_to_string(j_val_i);
      im_body711_res_append_string_data(body, name, buff);
      json_free_serialized_string(buff);
    }
      break;  

    case JSONNull: {
      DBG("[%s] in [JSONNull]", name);
    }
      break;  

    case JSONBoolean: {
		int boolVal = json_boolean(j_val_i);
      im_body711_res_append_bool_data(body, name, boolVal);
    }
      break;  

    case JSONArray: {
		JSON_Array  *j_arr = json_array  (j_val_i);
		int el_cnt = (int)json_array_get_count(j_arr);
		
		char *el_str_arr[100];
		double el_arr[100];

		JSON_Value_Type type_el = -1;

		double sample_number = 0.0;
		char* sample_str = NULL;

		for ( i=0; i<el_cnt; i++)
		{
			JSON_Value *j_val_el = json_array_get_value(j_arr, i);
			type_el = json_value_get_type (j_val_el);

			if ( type_el ==  JSONString )
			{
				const char *s_val = json_string(j_val_el);
				el_str_arr[i] = s_val;
				sample_str = s_val;
			} 
			else if ( type_el ==  JSONNumber )
			{
				double n_val = json_number(j_val_el);

				el_arr[i] = n_val;
				sample_number = n_val;
			}
			else if ( type_el ==  JSONBoolean )
			{
				int boolVal = json_boolean(j_val_el);
				el_arr[i] = (double)boolVal;
			}
			else {
				DBG("JSON_Value_Type is [%d]", type_el);
			}
		}

		if ( type_el ==  JSONString )
		{
			if ( __is_string_datetime(sample_str) )		{
				im_body711_res_append_string_datetimestr_list(body, name, el_cnt, el_str_arr);
			} else {
				im_body711_res_append_string_data_list(body, name, el_cnt, el_str_arr);
			}
		}
		else if ( type_el ==  JSONNumber )
		{
			if ( __is_number_integer(sample_number) )		{
				im_body711_res_append_int_data_list(body, name, el_cnt, el_arr);
			} else {
				im_body711_res_append_realnum_data_list(body, name, el_cnt, el_arr);
			}
		}
		else if ( type_el ==  JSONBoolean )
		{
	      im_body711_res_append_bool_data_list(body, name, el_cnt, el_arr);
		}
    }
      break;  

    default:
      WRN("j_type [%d] not supprted", type);
    }
  }


  json_value_free(j_val);

  return 0;
}




static int __if711_process_req_device(im_client_tPtr cli, IMPacketPtr res, IMPacketPtr req, char *devId, char *resource_name)
{
	char *prop_name;
	int idx = 0;

	//im_body711_res_append_colecRow(&res->body, resource_name);

#if 0
	while ( im_body711_req_get_cmdDataInfo_with_index(&req->body, idx, &prop_name) == 0  )
	{
		DBG("devId=[%s], prop_name=[%s]", devId, prop_name);
		if ( cli->if711_lastval_handler_for != NULL ) {
			cli->if711_lastval_handler_for(cli, devId, resource_name, &res->body);
		}
		idx++;
	}
	if ( idx == 0 && cli->if711_lastval_handler_for_all != NULL )
	{
		cli->if711_lastval_handler_for_all(cli, devId, &res->body);
	}
#else

	if ( resource_name == NULL && cli->if711_lastval_handler_for_all != NULL )
	{
		// groupTagCode가 명시되지 않은 711요청인 경우, 모든 센싱태그값으로 응답
		cli->if711_lastval_handler_for_all(cli, devId, &res->body);
	}
	else if ( resource_name != NULL && cli->if711_lastval_handler_for != NULL )
	{
		// groupTagCode가 명시된 711요청인 경우, 해당 태그그룹의 센싱태그값으로 응답
		cli->if711_lastval_handler_for(cli, devId, resource_name, &res->body);
	}

#endif

	return 0;
}


/** 
 *   @brief  example_if711_request_handler  
 */  
void im_packet711_request_handler_simple(im_client_tPtr cli, IMPacketPtr req)
{
  IMPacket res;

  unsigned long long trx_id = im_head_get_trmTransacId(&req->head);
  int rc = im_packet711_res_init(&res, cli->commChAthnNo, trx_id);
  if ( rc < 0 )    {
    ERR("im_packet711_res_init()");
    return;
  }

  im_body711_res_set_with_req(&res.body, &req->body);

  // 데이터 핸들링을 위한 초기화
  im_body711_req_start_parse(&req->body);

	char *resource_name = NULL;
	im_body711_req_get_groupTagCode(&req->body, &resource_name);
	if ( resource_name == NULL )	{
		DBG("====================================");
		DBG("= resource_name is NULL");
		DBG("====================================");
	} else {
		DBG("====================================");
		DBG("= resource_name=[%s]", resource_name);
		DBG("====================================");
	}

	
	char *devId;
	int idx = 0;
	while ( im_body711_req_get_inclDevId_with_index(&req->body, idx, &devId) == 0 ){
		DBG("devId=[%s]", devId);

		// 711_res_body->디바이스셋 추가
		im_body711_res_append_devColecData(&res.body, devId);
			
		__if711_process_req_device(cli, &res, req, devId, resource_name);

		idx++;
	}


  rc = im_transact_packet_with_response_handler(cli, &res, NULL);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&res);
    return;
  }

  im_packet_release(&res);

  return;
}

