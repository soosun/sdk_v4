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



#ifndef IOTMAKERS_CLIENT_H
#define IOTMAKERS_CLIENT_H

#include <stdio.h>
#include "platform.h"
#include "kt_iot_log.h"
#include "packet/packet.h"
#include "netio/net_context.h"


#define IM_CLIENT_VERSION       "4.5.6 2020-12-11"

#define IOTMAKERS_KEEPALIVE_IN_SEC          50
#define IOTMAKERS_MAX_CONTROL_DATA          32
#define IOTMAKERS_NET_READ_TIMEOUT_IN_SEC   3

/*
개방형 simple api용 : 수집전송 주기를 최대 1초로 제한
*/
#define IOTMAKERS_CIRCUIT_BREAKER_IN_SEC  1

/*
 * IoTMakers Client Instance
 *
 */
typedef struct im_client_t {

  char  dev_id[40];
  char  svc_gw[40];

  char  commChAthnNo[64+1];
  int isChAthnSuccess;

  /*
    장치제어 요청 핸들러
  */
  int failControlCnt;

	void (*if525_handler_for_packet)(void* cli, IMPacketPtr pkt);

	char* (*if525_handler_for_string) (void* cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value);
	int (*if525_handler_for_integer)(void* cli, char *dev_id, char *resource_name, char *prop_name, int prop_value);
	double (*if525_handler_for_float)  (void* cli, char *dev_id, char *resource_name, char *prop_name, double prop_value);
	int (*if525_handler_for_boolean)(void* cli, char *dev_id, char *resource_name, char *prop_name, int prop_value);
	int (*if525_handler_on_end_of_control)   (void* cli, char *dev_id, char *resource_name, void *report_body);

	// will be deprecated soon, 2020-10-13
	double (*if525_handler_for_number)(void* cli, char *dev_id, char *resource_name, char *prop_name, double prop_value);

  /*
    장치 최종값 조회 요청 핸들러
  */
	void (*if711_handler_for_packet)(void* cli, IMPacketPtr pkt);

	void (*if711_lastval_handler_for_all) (void* cli, char *dev_id, void* res_body);
	void (*if711_lastval_handler_for) (void* cli, char *dev_id, char *groupTagCode, void* res_body);

  /*
  개방형 simple api용 : 수집전송 주기를 최대 1초로 제한
  */
  int circuit_breaker_timestamp;

  connection_t conn_ctx;
  unsigned long packet_count_out;
  unsigned long packet_count_in;
  char *tls_ca_file;
  char *tls_ca_path;

  /*
  TCP 세션유지용
  */
  int keepalive_timestamp;
  int packet_count_out_check;


  /*
  circuit_breaker ON/OFF 스위치 (기본값:ON)
  */
  int circuit_breaker_sw;

  /*
  411 응답대기 ON/OFF 스위치 (기본값:ON)
  2020-04-17
  */
  int response_wait_sw;

    /*
    Java JNI사용시에 JNIEnv *env를 저장    
    */
    void *host_evn;
    void *host_object;

} im_client_t, *im_client_tPtr;


#ifdef __cplusplus
extern "C"
{
#endif


// client.c
int im_client_set_loglevel(int level);
int im_client_init(im_client_tPtr cli);
int im_client_set_tls_cafile(im_client_tPtr cli, char* ca_file);
int im_client_set_tls_capath(im_client_tPtr cli, char* ca_path);
int im_turn_circuit_breaker_off(im_client_tPtr cli);
int im_turn_response_wait_off(im_client_tPtr cli);
int im_connect_to_via_srcip(im_client_tPtr cli, char *ip_str, int port, char *srcip_str);
int im_connect_to(im_client_tPtr cli, char *ip_str, int port);
int im_connect(im_client_tPtr cli);
void im_disconnect(im_client_tPtr cli);

int im_poll(im_client_tPtr cli);
int im_transact_packet_with_response_handler(im_client_tPtr cli, IMPacketPtr req, void (*response_packet_handler)(im_client_tPtr, IMPacketPtr));
int im_auth_device_with_nic_desc(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw, char *mac_addr, char *nic_desc);
int im_auth_device(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw);
int im_session_keepalive(im_client_tPtr cli);



// packet_io.c
int im_conn_send_packet(im_client_tPtr cli, IMPacketPtr pkt);
int im_conn_recv_packet(im_client_tPtr cli, IMPacketPtr pkt);

// simple_api.c
int im_send_number_data_complex(im_client_tPtr cli, int count, ...);
int im_send_string_data_complex(im_client_tPtr cli, int count, ...);
int im_send_number_data(im_client_tPtr cli, char *prop_name, double prop_value);
int im_send_string_data(im_client_tPtr cli, char *prop_name, char *prop_value);
void im_packet525_request_handler_simple(im_client_tPtr cli, IMPacketPtr req);
int im_if525_rep_set_data_in_json(IMPacketBodyPtr body, char *resource_name, char *json_str);

int im_send_data(im_client_tPtr cli, char *json_str, char *resource_name);
int im_send_data_on_dev(im_client_tPtr cli, char *json_str, char *resource_name, char *dev_id);

// packet builder
int im_body224_req_set_macAdr(IMPacketBodyPtr body, char *mac_addr);
int im_body224_req_set_sessnDesc(IMPacketBodyPtr body, char *sess_desc);

// netio
void netio_set_read_head_length_4(connection_tPtr ctx);
int net_udp_send_buffer(connection_tPtr ctx);
int net_udp_recv_buffer(connection_tPtr ctx);
int net_udp_available_timeout(connection_tPtr ctx, int seconds);
int net_udp_available(connection_tPtr ctx);

// util
long im_util_gettimeofday_as_sec();

#ifdef __cplusplus
}
#endif


#endif
