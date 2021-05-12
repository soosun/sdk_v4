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


#include "packet/head/head.h"

int im_head000_req_release(IMPacketHeadPtr head)
{
  im_head_release(head);
	return 0;
}

  /** 서버 현재시간 조회 */
int im_head121_req_init(IMPacketHeadPtr head)
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_SVR_DT_REQ);
	return 0;
}

  /** 장치 TCP 채널 인증 */
int im_head224_req_init(IMPacketHeadPtr head )
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_ATHN_COMMCHATHN_DEV_TCP);
	return 0;
}


	/** KeepAlive TCP 채널 */
int im_head231_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_KEEP_ALIVE_COMMCHATHN_TCP);
  im_head_set_commChAthnNo(head, auth_code);
	return 0;
}


	/** 장치정보 갱신보고 */
int im_head332_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_INITA_DEV_UDATERPRT);
  im_head_set_commChAthnNo(head, auth_code);
	return 0;
}


	/** 수집 통합데이터 */
int im_head411_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_COLEC_ITGDATA_RECV);
  im_head_set_commChAthnNo(head, auth_code);
	return 0;
}

	/** 제어 수신 */
int im_head525_res_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid )
{
	im_head_init_response(head);
  im_head_set_methodType(head, MthdType_CONTL_ITGCNVY_DATA);
  im_head_set_commChAthnNo(head, auth_code);
  im_head_set_trmTransacId(head, txid);
	return 0;
}

int im_head525_rep_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid )
{
	im_head_init_report(head);
  im_head_set_methodType(head, MthdType_CONTL_ITGCNVY_DATA);
  im_head_set_commChAthnNo(head, auth_code);
  im_head_set_trmTransacId(head, txid);
	return 0;
}

	/** 최종값 조회 수신 */
int im_head711_res_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid )
{
	im_head_init_response(head);
  im_head_set_methodType(head, MthdType_LAST_VAL_QUERY);
  im_head_set_commChAthnNo(head, auth_code);
  im_head_set_trmTransacId(head, txid);
	return 0;
}

	/** OTP 확인 */
int im_head731_req_init(IMPacketHeadPtr head, char *auth_code )
{
	im_head_init_oneway_req(head);
  im_head_set_methodType(head, MthdType_OPT_CHECK_QUERY);
	return 0;
}




