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
#include "packet/packet.h"
#include "netio/net_context.h"
#include "client/client.h"

/**
  @pkt : 구성된 Packet for send
*/
int im_conn_send_packet(im_client_tPtr cli, IMPacketPtr pkt)
{
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;

#ifdef HAVE_HTTP
	IMPacketBodyPtr body = &pkt->body;
    int in_msg_len = im_body_get_serialized_to_buff(body,  pconn_ctx->send_buff.data, IOBUFF_MAX_SIZE);
    pconn_ctx->send_buff.length = in_msg_len;
#else
  int *p_meg_len = (int*)&(pconn_ctx->send_buff.data[0]);

  int in_msg_len = im_packet_get_serialized_to_buff(pkt, pconn_ctx->send_buff.data+4, IOBUFF_MAX_SIZE-4);
  *p_meg_len = htonl(in_msg_len);
  pconn_ctx->send_buff.length = 4 + in_msg_len;
#endif

  LOGHex("SEND >>>>>>>>", pconn_ctx->send_buff.data, (int)pconn_ctx->send_buff.length);
  int sent_bytes = netio_send_buffer(pconn_ctx);
  if ( sent_bytes != pconn_ctx->send_buff.length )  {
    ERR("write(); [%d] bytes", sent_bytes);
    return -1;
  }

  cli->packet_count_out++;

  return 0;
}


/**
  @pkt : 초기화된 Packet for recv
*/
int im_conn_recv_packet(im_client_tPtr cli, IMPacketPtr pkt)
{
  connection_tPtr pconn_ctx = (connection_tPtr)&cli->conn_ctx;

  int read_bytes = netio_recv_buffer(pconn_ctx);
  if ( read_bytes < 0 )      {
      ERR("read(); [%d] bytes", read_bytes);
      return -1;
  }
  LOGHex("RECV <<<<<<<<", pconn_ctx->recv_buff.data, (int)pconn_ctx->recv_buff.length);


#ifdef HAVE_HTTP
	IMPacketBodyPtr body = &pkt->body;

    int rc = im_body_get_deserialized_from_buff_len(body, &pconn_ctx->recv_buff.data[0], pconn_ctx->recv_buff.length);
    if ( rc < 0 )    {
        ERR("im_body_get_deserialized_from_buff_len(); rc=%d", rc);
        return -1;
    }

#else
  int rc = im_packet_get_deserialized_from_buff(pkt, pconn_ctx->recv_buff.data+4, pconn_ctx->recv_buff.length-4);
  if ( rc < 0 )    {
    ERR("im_packet_get_deserialized_from_buff(); rc=%d", rc);
    return -1;
  }
#endif

  cli->packet_count_in++;

  return 0;
}

