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


#include "packet/packet.h"

	
  /** 서버 현재시간 조회 */
int im_packet121_req_init(IMPacketPtr pkt)
{
	im_head121_req_init(&pkt->head);
	im_body121_req_init(&pkt->body);
	return 0;
}


	
  /** 장치 TCP 채널 인증 */
int im_packet224_req_init(IMPacketPtr pkt)
{
	im_head224_req_init(&pkt->head);
	im_body224_req_init(&pkt->body);

	return 0;
}

	
	/** KeepAlive TCP 채널 */
int im_packet231_req_init(IMPacketPtr pkt, char *commChAthnNo)
{
	im_head231_req_init(&pkt->head, commChAthnNo);
	im_body231_req_init(&pkt->body);

	return 0;
}

	
	/** 장치정보 갱신보고 */
int im_packet332_req_init(IMPacketPtr pkt, char *commChAthnNo)
{
	im_head332_req_init(&pkt->head, commChAthnNo);
	im_body332_req_init(&pkt->body);
	return 0;
}


	
	/** 수집 통합데이터 */
int im_packet411_req_init(IMPacketPtr pkt, char *commChAthnNo)
{
	im_head411_req_init(&pkt->head, commChAthnNo);
	im_body411_req_init(&pkt->body);
	return 0;
}

	
	/** 제어 결과 통보 */
int im_packet525_rep_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid)
{
	im_head525_rep_init(&pkt->head, commChAthnNo, txid);
	im_body525_rep_init(&pkt->body);
	return 0;
}

	/** 제어 수신 응답*/
int im_packet525_res_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid)
{
	im_head525_res_init(&pkt->head, commChAthnNo, txid);
	im_body525_res_init(&pkt->body);
	return 0;
}
	
	
	/** 최종값 통보 */
int im_packet711_res_init(IMPacketPtr pkt, char *commChAthnNo, unsigned long long txid)
{
	im_head711_res_init(&pkt->head, commChAthnNo, txid);
	im_body711_res_init(&pkt->body);
	return 0;
}

	
	/** OTP 확인 */
int im_packet731_req_init(IMPacketPtr pkt, char *commChAthnNo)
{
	im_head731_req_init(&pkt->head, commChAthnNo);
	im_body731_req_init(&pkt->body);
	return 0;
}

