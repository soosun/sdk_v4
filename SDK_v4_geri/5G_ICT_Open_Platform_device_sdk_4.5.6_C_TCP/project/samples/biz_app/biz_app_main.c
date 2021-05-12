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


// 서버시각 조회
int example_if121_req_server_time(im_client_tPtr cli);

// OTP서비스 장치 예
int example_if731_send_otp_check(im_client_tPtr cli);

// 수집전송: 문자/숫자 복합데이터 전송
int example_if411_send_complex_data(im_client_tPtr cli);

// 수집전송: 데이터를 JSON으로 구성하는 예
int example_if411_send_data_in_json_string(im_client_tPtr cli);

// 수집전송: 다수의 데이터 묶음 전송
int example_if411_send_bulk_data(im_client_tPtr cli);

// 수집전송: 시간별 다수의 데이터 묶음 전송
int example_if411_send_bulk_data_with_occDt(im_client_tPtr cli);

// 수집전송: 다수의 데이터 묶음 전송(그룹태그 명시)
int example_if411_send_bulk_data_with_groupTag(im_client_tPtr cli);

// 수집전송: 수집장치(하위 디바이스) 아이디 명시
int example_if411_send_data_with_deviceid(im_client_tPtr cli);


// 제어수신 : 문자열 타입 제어데이터 핸들러
extern char* example_if525_request_handler_for_string(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value);
// 제어수신 : 정수 타입 제어데이터 핸들러
extern int example_if525_request_handler_for_integer(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value);
// 제어수신 : 실수 타입 제어데이터 핸들러
extern double example_if525_request_handler_for_float(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, double prop_value);
// 제어수신 : 부울 타입 제어데이터 핸들러
extern int example_if525_request_handler_for_boolean(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value);
// 제어수신 : (다수의) 제어데이터 처리 완료시 처리 핸들러
extern int example_if525_handler_on_end_of_control(im_client_tPtr cli, char *dev_id, char *resource_name);


// 최종값조회수신 : 그룹태그가 명시된 경우 처리 핸들러
extern void example_if711_lastval_handler_for(im_client_tPtr cli, char *dev_id, char *prop_name, void* res_body);
// 최종값조회수신 : 그룹태그가 명시되지 않은 경우 처리 핸들러
extern void example_if711_lastval_handler_for_all(im_client_tPtr cli, char *dev_id, void* res_body);


//////////////////////////////////////////////////////////////////////////
//

/** 
 *  수집데이터 전송 예  
 */  
static int example_send_data_in_json_string(im_client_tPtr cli)
{
#if 0
  char *json_str = "{\"Temp\":25.5, \"status\":\"hello\"}";

#else
	/*
  char *json_str = "{\"Temp\":25.5, "
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
	*/

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
	 *  하위장치의 수집데이터 전송 예  
	 ************************************/  
	char *dev_id = "sub_dev";
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



/** 
 *   @brief  biz_app MAIN
 *   Usage : biz_app {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}
 */  
int main(int argc, char *argv[])
{
  int rc = 0;
  
 
  /********************************************************
  *  IoTMakers 디바이스 클라이언트 핸들러
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
  * 로그레벨 설정
  *   0=NoLog, 1=Error, 2=Info, 3=Debug
  ********************************************************/
  im_client_set_loglevel(3);


  /********************************************************
  *  핸들러 초기화
  ********************************************************/
  rc = im_client_init(&client);
  if ( rc < 0 )  {
    return -1;
  }

  /********************************************************
  *  디바이스 제어수신 처리기 등록 (제어 핸들러)
  ********************************************************/
  client.if525_handler_for_string = example_if525_request_handler_for_string;
  client.if525_handler_for_integer = example_if525_request_handler_for_integer;
  client.if525_handler_for_float = example_if525_request_handler_for_float;
  client.if525_handler_for_boolean = example_if525_request_handler_for_boolean;
  client.if525_handler_on_end_of_control = example_if525_handler_on_end_of_control;

  /********************************************************
  *  디바이스 최종값수신 처리기 등록 (최종값 핸들러)
  ********************************************************/
  client.if711_lastval_handler_for = example_if711_lastval_handler_for;
  client.if711_lastval_handler_for_all = example_if711_lastval_handler_for_all;

  /********************************************************
  *  접속 종료시 이 지점부터 다시 시작
  ********************************************************/
_AGAIN:

  /********************************************************
  *   서버인증서 확인 불필요시에는 생략가능
  ********************************************************/
#ifdef HAVE_OPENSSL
    // DEBIAN, UBUNTU 계열
    //im_client_set_tls_cafile(&client, "/etc/ssl/certs/XRamp_Global_CA_Root.pem");
    //im_client_set_tls_cafile(&client, "/etc/ssl/certs/ca-certificates.crt");
    im_client_set_tls_capath(&client, "/etc/ssl/certs");
    //im_client_set_tls_cafile(&client, "./ca-certificates.crt");
#endif

  /********************************************************
  *   EC 접속 주소 지정 (TCP 표준 인터페이스)
  ********************************************************/
  rc = im_connect_to(&client, ec_ip, ec_port);
  if ( rc < 0 )  {
    return -1;
  }

  /********************************************************
  *  디바이스 인증 요청
  ********************************************************/
  rc = im_auth_device(&client, dev_id, dev_pw, svc_gw);
  if ( rc < 0 )  {
    im_disconnect(&client);
    return -1;
  }

  /********************************************************
  *  예제) 서버시각 요청
  ********************************************************/
  example_if121_req_server_time(&client);


#if 0
  im_turn_circuit_breaker_off(&client);
#endif


  while ( 1==1)  {

    /********************************************************
    *  예제) 수집데이터 전송 
    ********************************************************/
    
	rc = example_send_data_in_json_string(&client);
    if ( rc < 0 )  {
      // 전송실패시 loop종료
      break;
    }

    im_msleep(1*1000);

    /********************************************************
    *  송수신 이벤트 폴링
    ********************************************************/
    rc = im_poll(&client);
    if ( rc < 0 )  {
      // 이벤트 처리 실패시 loop종료
      break;
    }
  }

  /********************************************************
  *  접속 해제
  ********************************************************/
  im_disconnect(&client);


  /********************************************************
  *  3초 후에 재접속 시도
  ********************************************************/
  im_msleep(3*1000);
  goto _AGAIN;

  return 0;
}
