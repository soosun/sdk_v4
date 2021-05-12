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
 *   @brief  example_if411_send_bulk_data_with_groupTag  
 */  
int example_if411_send_bulk_data_with_groupTag(im_client_tPtr cli)
{
  IMPacket req;

  int rc = im_packet411_req_init(&req, cli->commChAthnNo);
  if ( rc < 0 )    {
    ERR("im_packet411_req_init()");
    return -1;
  }

  im_body411_req_set_extrSysId(&req.body, cli->svc_gw);
  im_body411_req_append_devColecData(&req.body, cli->dev_id);


  char *resource_name = "GROUP01";
  im_body411_req_append_colecRow(&req.body, NULL, resource_name);
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


/** 
 *   @brief  example_if411_send_bulk_data_with_groupTag  in JSON
 */  
int example_if411_send_bulk_data_with_groupTag_in_json(im_client_tPtr cli)
{
  char *json_str = "{\"Temp\":25.5, \"status\":\"hello\"";
  char *dev_id = "sub_device_001";
  char *resource_name = "GROUP01";

  int rc = im_send_data_on_dev(cli, json_str, resource_name, dev_id);
  if ( rc < 0)  {
    ERR("fail im_send_data()");
    return -1;
  }

  return 0;
}


