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
#include "util/util.h"
#include "client/client.h"

/**
  개방형IoT플랫폼
*/
#define IM_EC_IP  "220.90.216.90"

#ifdef HAVE_MQTT
#define IM_EC_PORT  10030
#elif HAVE_HTTP
#define IM_EC_PORT  10010
#else 
#define IM_EC_PORT  10020
#endif

int g_log_level = 3;


//////////////////////////////////////////////////////////////////////////
int im_client_set_loglevel(int level)
{
  g_log_level = level;
  return 0;
}

int im_client_init(im_client_tPtr cli)
{
  INF("IoTMakers STD Device SDK ver. %s", IM_CLIENT_VERSION);

  memset(cli, 0x00, sizeof(im_client_t));
  cli->keepalive_timestamp = im_util_gettimeofday_as_sec(); 
  cli->circuit_breaker_sw = (1); 
  cli->response_wait_sw = (1); 

  return 0;
}

int im_client_set_tls_cafile(im_client_tPtr cli, char* ca_file)
{
  cli->tls_ca_file = ca_file;
  cli->tls_ca_path = NULL;
  return 0;
}

int im_client_set_tls_capath(im_client_tPtr cli, char* ca_path)
{
  cli->tls_ca_file = NULL;
  cli->tls_ca_path = ca_path;
  return 0;
}


int im_turn_circuit_breaker_off(im_client_tPtr cli)
{
  cli->circuit_breaker_sw = (0); 
  return 0;
}

int im_turn_response_wait_off(im_client_tPtr cli)
{
  cli->response_wait_sw = (0); 
  return 0;
}


int im_connect_to(im_client_tPtr cli, char *ip_str, int port)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsa_data;
	WSAStartup(wVersionRequested, &wsa_data);
#endif

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  if ( cli->tls_ca_file != NULL )    {
      net_tls_set_trusred_ca_file(conn_ctx, cli->tls_ca_file) ;
  }

  if ( cli->tls_ca_path != NULL )    {
      net_tls_set_trusred_ca_path(conn_ctx, cli->tls_ca_path) ;
  }

  int rc = net_tls_open_socket(conn_ctx);
  if ( rc < 0 )  {
    ERR("fail net_tls_open_socket()");
    return -1;
  }

  rc = net_tls_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tls_connect_ipv4()");
    return -1;
  }

#elif HAVE_MQTT
  int rc = net_mqtt_connect(conn_ctx, ip_str, port, IOTMAKERS_KEEPALIVE_IN_SEC);
  if ( rc < 0 )  {
    ERR("fail net_mqtt_connect()");
    return -1;
  }

#elif HAVE_HTTP
  int rc = net_http_connect(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_http_connect()");
    return -1;
  }
  return 0;

#else
  int rc = net_tcp_open_socket(conn_ctx);
  if ( rc < 0 )  {
    ERR("fail net_tcp_open_socket()");
    return -1;
  }

  rc = net_tcp_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#endif

  return 0;
}


int im_connect_to_via_srcip(im_client_tPtr cli, char *ip_str, int port, char *srcip_str)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsa_data;
	WSAStartup(wVersionRequested, &wsa_data);
#endif

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  int rc = net_tls_open_socket_n_bind_with_ip(conn_ctx, srcip_str);
  if ( rc < 0 )  {
    ERR("fail net_tls_init()");
    return -1;
  }

  rc = net_tls_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#else
  int rc = net_tcp_open_socket_n_bind_with_ip(conn_ctx, srcip_str);
  if ( rc < 0 )  {
    ERR("fail net_tcp_open_socket()");
    return -1;
  }

  rc = net_tcp_connect_ipv4(conn_ctx, ip_str, port);
  if ( rc < 0 )  {
    ERR("fail net_tcp_connect_ipv4()");
    return -1;
  }
#endif
  return 0;
}





int im_connect(im_client_tPtr cli)
{
  return im_connect_to(cli, IM_EC_IP, IM_EC_PORT);
}


void im_disconnect(im_client_tPtr cli)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

#if defined(HAVE_OPENSSL) || defined(HAVE_MBEDTLS)
  net_tls_disconnect(conn_ctx);

#elif HAVE_MQTT
  net_mqtt_disconnect(conn_ctx);

#elif HAVE_HTTP
  net_http_disconnect(conn_ctx);

#else
  net_tcp_disconnect(conn_ctx);
  net_tcp_close_socket(conn_ctx);
#endif

#ifdef WIN32
	WSACleanup();
#endif
}

////////////////////////////////////////////////////////////
static void request_packet525_handler(im_client_tPtr cli, IMPacketPtr req);
static void request_packet711_handler(im_client_tPtr cli, IMPacketPtr req);

static void packet_handler(im_client_tPtr cli, IMPacketPtr ppkt)
{
  unsigned long long pkt_trx = im_head_get_trmTransacId(&ppkt->head) ;
  int methodTypeReq = im_head_get_methodType(&ppkt->head);

  //DBG("methodType [%d], trxid=[%ld]", methodTypeReq, pkt_trx);

  switch (methodTypeReq)	{
    case 525:	// MthdType_CONTL_ITGCNVY_DATA
      request_packet525_handler(cli, ppkt);
      break;
    case 711:	// MthdType_LAST_VAL_QUERY
      request_packet711_handler(cli, ppkt);
      break;
    case 411:	// 수집전송에 대한 응답
      WRN("Res for methodType[411]");
      break;
    
    default:
    WRN("Not Supported methodType [%d]", methodTypeReq);
  }

}


static int process_packet_from_server(im_client_tPtr cli)
{
  IMPacket pkt;
  IMPacketPtr ppkt = (IMPacketPtr)&pkt;

  int rc = im_packet_init(ppkt);
  if ( rc < 0 )    {
    ERR("im_packet_init()");
    return -1;
  }

  rc = im_conn_recv_packet(cli, ppkt);
  if ( rc < 0 )  {
    im_packet_release(ppkt);
    return -1;
  }

  // Call response_packet_handler()
  packet_handler(cli, ppkt);

  im_packet_release(ppkt);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

int im_poll(im_client_tPtr cli)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;
  int rc;


#if !defined(HAVE_MQTT) && !defined(HAVE_HTTP)

  if ( IOTMAKERS_KEEPALIVE_IN_SEC > 0 )
  {
    // 최근 IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC 이내에
    if ( (im_util_gettimeofday_as_sec() - cli->keepalive_timestamp) >  IOTMAKERS_KEEPALIVE_IN_SEC) 
    {
      cli->keepalive_timestamp = im_util_gettimeofday_as_sec(); 
#if 1
      // 패킷 송신 내역이 없으면
      if ( cli->packet_count_out_check == cli->packet_count_out )        {
        // 접속유지 요청
        if ( im_session_keepalive(cli) < 0 )        {
          return -1;
        };
      }
      cli->packet_count_out_check = cli->packet_count_out; 
#else
      if ( im_session_keepalive(cli) < 0 )        {
        return -1;
      };
#endif
    }
  }

#endif

check_again:
  // 최대 1 msec 동안 블로킹됨
  rc = netio_available(conn_ctx);
  if ( rc < 0 ) {
    ERR("fail sock");
    return -1;
  }

  if ( rc == 0 ) {
    //DBG("no packet to read");
    DBG("sent=[%ld]pkts, recv=[%ld]pkts", cli->packet_count_out, cli->packet_count_in);
    return 0;
  }

  rc = process_packet_from_server(cli);
  if ( rc < 0 ) {
    ERR("fail process_packet_from_server()");
    return -1;
  }

  goto check_again;

  return 0;
}



int im_transact_packet_with_response_handler(im_client_tPtr cli, IMPacketPtr req, 
                                              void (*response_packet_handler)(im_client_tPtr, IMPacketPtr))
{
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;

  int rc = im_conn_send_packet(cli, req);
  if ( rc < 0 )  {
    return -1;
  }

  if ( response_packet_handler == NULL )  {
    // 응답수신 불필요  
    // im_poll()에서 처리됨
    return 0;
  }

wait_data:
  // IOTMAKERS_NET_READ_TIMEOUT_IN_SEC 동안 블로킹
  if ( netio_available_timeout(pconn_ctx, IOTMAKERS_NET_READ_TIMEOUT_IN_SEC) <= 0 ) {
    ERR("no data to read");
    return -1;
  }

  IMPacket res;
  IMPacketPtr pres = (IMPacketPtr)&res;

  rc = im_packet_init(pres);
  if ( rc < 0 )    {
    im_packet_release(req);
    ERR("im_packet_init()");
    return -1;
  }

  rc = im_conn_recv_packet(cli, pres);
  if ( rc < 0 )  {
    im_packet_release(req);
    ERR("im_conn_recv_packet()");
    return -1;
  }

#ifndef HAVE_HTTP
  unsigned long long req_trx, res_trx;
  req_trx = im_head_get_trmTransacId(&req->head) ;
  res_trx = im_head_get_trmTransacId(&pres->head) ;
  
  //DBG("trx[%ld][%ld]", req_trx, res_trx);
  if ( req_trx != res_trx )  {
    WRN("trx misMatch; [%lld][%lld]", req_trx, res_trx);

    // 트랜잭션ID가 다르므로 별도의 핸들러에서 처리
    packet_handler(cli, pres);

    im_packet_release(pres);
    goto wait_data;
  }
#endif

  // Call response_packet_handler() back
  response_packet_handler(cli, pres);

  im_packet_release(pres);

  return 0;
}


///////////////////////////////////////////////////////////////////
/** 224 장치 인증 */
///////////////////////////////////////////////////////////////////
static void response_packet_handler_224(im_client_tPtr cli, IMPacketPtr res)
{
  strcpy(cli->commChAthnNo, (char*)im_body224_res_body_get_athnNo(&res->body));
  WRN("athnNo=[%s]", cli->commChAthnNo);
  cli->isChAthnSuccess = (1);

#ifdef HAVE_HTTP
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;
  net_http_set_auth_tok(conn_ctx, cli->commChAthnNo);
#endif
}

int im_auth_device_with_nic_desc(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw, char *mac_addr, char *nic_desc)
{
  IMPacket req;

  int rc = im_packet224_req_init(&req);
  if ( rc < 0 )    {
    ERR("im_packet_224_req_init()");
    return -1;
  }

  im_body224_req_set_devId(&req.body, dev_id);
  im_body224_req_set_athnRqtNo(&req.body, dev_pw);
  im_body224_req_set_extrSysId(&req.body, svc_gw);

  if ( mac_addr != NULL)  {
    im_body224_req_set_macAdr(&req.body, mac_addr);
  }

  if ( nic_desc != NULL)  {
    im_body224_req_set_sessnDesc(&req.body, nic_desc);
  }

  cli->isChAthnSuccess = (0);

  rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_224);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    ERR("auth_device seems to be failed");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  
  if ( cli->isChAthnSuccess == (0) )  {
    ERR("fail auth_device");
    return -1;
  }

  strcpy(cli->dev_id, dev_id);
  strcpy(cli->svc_gw, svc_gw);

  /* 
    set for netio module to read first 4byte for length
  */
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;
  netio_set_read_head_length_4(pconn_ctx);

  return 0;
}


int im_auth_device(im_client_tPtr cli, char *dev_id, char *dev_pw, char *svc_gw)
{
  connection_tPtr conn_ctx = (connection_tPtr)&cli->conn_ctx;

  IMPacket req;

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, dev_id);
#endif

  int rc = im_auth_device_with_nic_desc(cli, dev_id, dev_pw, svc_gw, NULL, NULL);
  if ( rc < 0 )    {
    ERR("im_auth_device_with_nic_desc()");
    return -1;
  }

#ifdef HAVE_MQTT
  net_mqtt_set_io_topic(conn_ctx, cli->commChAthnNo);
#endif


  return 0;
}



///////////////////////////////////////////////////////////////////
/** 231 접속유지 */
///////////////////////////////////////////////////////////////////
static void response_packet_handler_231(im_client_tPtr cli, IMPacketPtr res)
{
  // DUMMY
}

int im_session_keepalive(im_client_tPtr cli)
{
  IMPacket req;

  //im_packet231_req_init (장비인증토큰)
  int rc = im_packet231_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet231_req_init()");
    return -1;
  }

  rc = im_transact_packet_with_response_handler(cli, &req, response_packet_handler_231);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  
  return 0;
}

///////////////////////////////////////////////////////////////////

static int im_client_send_525_res_ok(im_client_tPtr cli, unsigned long long txid)
{
  IMPacket res;

  int rc = im_packet525_res_init(&res, cli->commChAthnNo, txid);
  if ( rc < 0 )    {
    ERR("im_packet_525_res_init()");
    return -1;
  }

	im_body525_res_set_respCd(&res.body, "100");
	im_body525_res_set_respMsg(&res.body, "SUCCESS");

  // Does not handle response
  rc = im_conn_send_packet(cli, &res);
  if ( rc < 0 )    {
    im_packet_release(&res);
    ERR("im_client_send_packet()");
    return -1;
  }

  im_packet_release(&res);
  return rc;
}


  /** 525 제어요청 handler*/
static void request_packet525_handler(im_client_tPtr cli, IMPacketPtr req)
{
  // 수신 OK 응답
  int rc = im_client_send_525_res_ok(cli, im_head_get_trmTransacId(&req->head));
  if ( rc < 0 )    {
    ERR("im_client_send_525_res_ok()");
    return;
  }

  // 파싱 & Handler call

  if ( cli->if525_handler_for_packet != NULL  )  {
    cli->if525_handler_for_packet(cli, req);

  } else {
    im_packet525_request_handler_simple(cli, req);
  }

  return;
}

  /** 711 최종값 조회 요청 handler*/
static void request_packet711_handler(im_client_tPtr cli, IMPacketPtr req)
{
  // 파싱 & Handler call
  if ( cli->if711_handler_for_packet != NULL  )  {
    cli->if711_handler_for_packet(cli, req);

  } else 	{
    im_packet711_request_handler_simple(cli, req);
  }

  return;
}


