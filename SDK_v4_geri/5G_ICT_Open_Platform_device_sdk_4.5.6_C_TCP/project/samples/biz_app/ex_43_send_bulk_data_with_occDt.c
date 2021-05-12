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

/** 
 *   @brief  on_411_response_arrived
 */  
static void on_411_response_arrived(im_client_tPtr cli, IMPacketPtr res)
{
  unsigned long long trx_id = im_head_get_trmTransacId(&res->head) ;
  //
  DBG("trx_id[%lld]", im_head_get_trmTransacId(&res->head));
  DBG("respCd[%s]", im_body411_res_get_respCd(&res->body));
}


/** 
 *   @brief  example_if411_send_bulk_data_with_occDt  
 */  
int example_if411_send_bulk_data_with_occDt(im_client_tPtr cli)
{
  IMPacket req;

  int rc = im_packet411_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  im_body411_req_set_extrSysId(&req.body, cli->svc_gw);
  im_body411_req_append_devColecData(&req.body, cli->dev_id);

  // 1번째 벌크
  char *occDt = "2017-10-18 10:29:42.100";
  im_body411_req_append_colecRow(&req.body, occDt, NULL);
  im_body411_req_set_realnum_data(&req.body, "number", (double)99.999);
  im_body411_req_set_string_data(&req.body, "string", "hello");
  im_body411_req_set_int_data(&req.body, "hum", (int)55);


  // 2번째 벌크, 1분 후
  occDt = "2017-10-18 10:30:42.100";
  im_body411_req_append_colecRow(&req.body, occDt, NULL);
  im_body411_req_set_realnum_data(&req.body, "number", (double)99.999);
  im_body411_req_set_string_data(&req.body, "string", "hello");
  im_body411_req_set_int_data(&req.body, "hum", (int)55);

  
  DBG("trx_id[%lld]", im_head_get_trmTransacId(&req.head));

  rc = im_transact_packet_with_response_handler(cli, &req, on_411_response_arrived);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  return 0;
}

