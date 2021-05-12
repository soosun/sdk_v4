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
static void on_731_response_arrived(im_client_tPtr cli, IMPacketPtr res)
{
  unsigned long long trx_id = im_head_get_trmTransacId(&res->head) ;
  //
  DBG("trx_id[%lld]", im_head_get_trmTransacId(&res->head));
  DBG("respCd[%s]", im_body411_res_get_respCd(&res->body));

  DBG("otpRespCode=[%s]", (char*)im_body731_res_get_otpRespCode(&res->body));
  DBG("respMessage=[%s]", (char*)im_body731_res_get_otpRespMessage(&res->body));
  DBG("phoneNumber=[%s]", (char*)im_body731_res_get_phoneNumber(&res->body));
}


/** 
 *   @brief  example_if731_send_otp_check
 */  
int example_if731_send_otp_check(im_client_tPtr cli)
{
  IMPacket req;
  char buff[512];
  int pkt_length = 0;

  //im_packet411_req_init (장비인증토큰)
  int rc = im_packet731_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  im_body731_req_set_extrSysId(&req.body, cli->svc_gw);
  im_body731_req_set_devId(&req.body, cli->dev_id);

  im_body731_req_set_seed(&req.body, "my_seed_string");
  im_body731_req_set_otp(&req.body, "my_otp_string");

  DBG("trx_id[%lld]", im_head_get_trmTransacId(&req.head));

  rc = im_transact_packet_with_response_handler(cli, &req, on_731_response_arrived);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  return 0;
}

