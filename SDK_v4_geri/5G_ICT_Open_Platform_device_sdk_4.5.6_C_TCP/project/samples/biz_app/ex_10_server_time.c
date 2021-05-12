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
 *   @brief  on_121_response_arrived
		im_client_121_req_server_time()의 응답패킷을 처리하는 콜백함수
 *  
 *   @param  cli is a pointer of IMClient
 *   @param  res is a pointer of IMPacket
 *   @return void
 */  
static void on_121_response_arrived(im_client_tPtr cli, IMPacketPtr res)
{
  DBG("srvrDt[%s]", im_body121_res_get_srvrDt(&res->body));
  DBG("srvrTimeZone[%s]", im_body121_res_get_srvrTimeZone(&res->body));
}


/** 
 *   @brief  sample_if121_req_server_date  
		IoTMakers 서버 시간을 요청하는 샘플 코드
        IoTMakers EC Interface_121을 호출합니다.
 *  
 *   @param  cli is a pointer of im_client_t
 *   @return 0
 */  
int example_if121_req_server_time(im_client_tPtr cli)
{
  IMPacket req;

  int rc = im_packet121_req_init(&req);
  if ( rc < 0 )    {
    ERR("im_packet121_req_init()");
    return -1;
  }

  rc = im_transact_packet_with_response_handler(cli, &req, on_121_response_arrived);
  if ( rc < 0 )    {
    ERR("fail im_transact_packet_with_response_handler()");
    im_packet_release(&req);
    return -1;
  }

  im_packet_release(&req);
  return 0;
}

