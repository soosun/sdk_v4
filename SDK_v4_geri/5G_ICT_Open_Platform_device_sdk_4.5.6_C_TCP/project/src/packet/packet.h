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


#ifndef IOTMAKERS_PACKET_H
#define IOTMAKERS_PACKET_H

#include "packet/head/head.h"
#include "packet/body/body.h"


typedef struct {
	IMPacketHead		head;
	IMPacketBody		body;
} IMPacket, *IMPacketPtr;


#ifdef __cplusplus
extern "C"
{
#endif

// packet.c 
int im_packet_init(IMPacketPtr pkt) ;
int im_packet_release(IMPacketPtr pkt) ;
int im_packet_get_serialized_to_buff(IMPacketPtr pkt, char *o_buff, unsigned int o_buff_len);
int im_packet_get_deserialized_from_buff(IMPacketPtr pkt, char *i_buff, unsigned int i_buff_len);


// packet_all_init.c 
int im_packet121_req_init(IMPacketPtr pkt);
int im_packet224_req_init(IMPacketPtr pkt);
int im_packet231_req_init(IMPacketPtr pkt, char *commChAthnNo);
int im_packet332_req_init(IMPacketPtr pkt, char *commChAthnNo);
int im_packet411_req_init(IMPacketPtr pkt, char *commChAthnNo);
int im_packet525_rep_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid);
int im_packet525_res_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid);
int im_packet711_res_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid);
int im_packet731_req_init(IMPacketPtr pkt, char *commChAthnNo);


#ifdef __cplusplus
}
#endif

#endif


