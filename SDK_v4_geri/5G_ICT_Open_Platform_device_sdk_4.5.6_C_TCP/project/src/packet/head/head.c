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
#include <stdlib.h>
#include <string.h>


#include "kt_iot_log.h"
#include "deps/platform.h"
#include "util/util.h"
#include "packet/head/head.h"


/**
  멀티세션 복제전송시에 trx_id를 공유한다.
*/
static unsigned long long g_trx_id_4_share = 0;

int im_head_set_trx_id_4_share()
{
  g_trx_id_4_share = (unsigned long long)im_util_get_unique_number();
  return 0;
}

int im_head_reset_trx_id_4_share()
{
  g_trx_id_4_share = (unsigned long long)0;
  return 0;
}

static unsigned long long get_new_tranx_id() 
{
  if ( g_trx_id_4_share != (unsigned long long)0 )  {
#if 0
  printf("!!!!!!!!!!!!!!!!![%ld]!!!!!!!!!!!!!!!\n", g_trx_id_4_share);
#endif    
    return g_trx_id_4_share;
  }
	return (unsigned long long)im_util_get_unique_number();
}



void im_head_init(IMPacketHeadPtr head)
{
	IMHeadRawPtr hraw = &(head->headRaw);

	memset(head, 0x00, sizeof(IMPacketHead));

	hraw->protoMainVer		= IOTMAKERS_PROTOCOL_VER_MAJOR;	// 프로토콜 메인버전
	hraw->protoSubVer		= IOTMAKERS_PROTOCOL_VER_MINOR;	// 프로토콜 하위버전
	hraw->hdrType			= HdrType_BASIC;				// 헤더타입
	hraw->hdrLen			= htons(IOTMAKERS_DEF_HEAD_LEN);	// 헤더 길이

	im_head_set_msgType(head, MsgType_REQUEST);			// 메시지 유형
	im_head_set_msgExchPtrn(head, MsgExchPtrn_NONE);		// 메시지 교환 패턴
	im_head_set_methodType(head, MthdType_NONE);			// 기능유형

	hraw->trmTransacId		= get_new_tranx_id();			// 트랜잭션아이디
	
#if 0
    memcpy(hraw->commChAthnNo, (char*)im_base_get_commChAthnNo(), 16);
#else
    memset(hraw->commChAthnNo, 0x00, 16);
#endif

	hraw->encryUsage		= UseYn_NO;						// 암호화사용여부
	hraw->encryType			= EcodType_AES_128;				// 암호화유형
	
	hraw->compUsage			= UseYn_NO;						// 압축사용여부
	hraw->compType			= CmpreType_HUFFMAN;			// 압축유형
	
	hraw->encodType			= EncdngType_JSON;				// 인코딩유형
	hraw->rsltCd			= RespCd_NONE;					// 결과코드

	head->ext_cnt			= 0;		// 헤더확장필드수
	return;
}

void im_head_init_oneway_req(IMPacketHeadPtr head)
{
	im_head_init(head);
	im_head_set_msgType(head, MsgType_REQUEST);
	im_head_set_msgExchPtrn(head, MsgExchPtrn_ONE_WAY_ACK); 
}
void im_head_init_response(IMPacketHeadPtr head)
{
	im_head_init(head);
	im_head_set_msgType(head, MsgType_RESPONSE);
}
void im_head_init_report(IMPacketHeadPtr head)
{
	im_head_init(head);
	im_head_set_msgType(head, MsgType_REPORT);
	im_head_set_msgExchPtrn(head, MsgExchPtrn_ONE_WAY);
}


void im_head_release(IMPacketHeadPtr head)
{
	IMHeadExtPtr ext = &(head->ext);
	im_headExt_release(ext);
}

void im_head_copy_trmTransacId(IMPacketHeadPtr headDest, IMPacketHeadPtr headSrc)
{
	headDest->headRaw.trmTransacId = headSrc->headRaw.trmTransacId;
}

/* =====================================
head get/set
====================================== */
void im_head_set_msgType(IMPacketHeadPtr head, MsgType type)
{
	head->headRaw.ugly.msgInfo.msgType = type;		// 메시지 유형
}
MsgType im_head_get_msgType(IMPacketHeadPtr head)
{
	return (MsgType)head->headRaw.ugly.msgInfo.msgType;		// 메시지 유형
}

void im_head_set_msgExchPtrn(IMPacketHeadPtr head, MsgExchPtrn type)
{
	head->headRaw.ugly.msgInfo.msgExchPtrn = type;		// 메시지 교환 패턴
}
MsgExchPtrn im_head_get_msgExchPtrn(IMPacketHeadPtr head)
{
	return (MsgExchPtrn)head->headRaw.ugly.msgInfo.msgExchPtrn;		// 메시지 교환 패턴
}

void im_head_set_methodType(IMPacketHeadPtr head, MthdType type)
{
#if defined(REVERSED)
	head->headRaw.ugly.methodType |=  (htons(type)& 0x0FFF) ;	
#else
	// mask least 12bits(0xFF0F) in network byte order of 16 bits
	head->headRaw.ugly.methodType |=  (htons(type)& 0xFF0F) ;	
#endif
}
MthdType im_head_get_methodType(IMPacketHeadPtr head)
{
#if defined(REVERSED)
	return (MthdType)ntohs(head->headRaw.ugly.methodType & 0x0FFF) ;	
#else
	// mask least 12bits(0xFF0F) in network byte order of 16 bits
	return (MthdType)ntohs(head->headRaw.ugly.methodType & 0xFF0F) ;	
#endif
}


void im_head_set_trmTransacId(IMPacketHeadPtr head, unsigned long long trxid)
{
	head->headRaw.trmTransacId	= trxid;
}
unsigned long long im_head_get_trmTransacId(IMPacketHeadPtr head)
{
	return head->headRaw.trmTransacId;
}


void im_head_set_commChAthnNo(IMPacketHeadPtr head, char *authCode)
{
	memcpy(head->headRaw.commChAthnNo, authCode, 16);
}
char* im_head_get_commChAthnNo(IMPacketHeadPtr head)
{
	return head->headRaw.commChAthnNo;
}




void im_head_set_respCd(IMPacketHeadPtr head, RespCd rcode)
{
	head->headRaw.rsltCd = htons(rcode);
}
RespCd im_head_get_respCd(IMPacketHeadPtr head)
{
	return (RespCd)ntohs(head->headRaw.rsltCd);
}

void im_head_set_hdrLen(IMPacketHeadPtr head, unsigned short len)
{
	DBG("len=[%d]", len);
	head->headRaw.hdrLen = htons(len);
}
unsigned short im_head_get_hdrLen(IMPacketHeadPtr head)
{
	return ntohs(head->headRaw.hdrLen);
}

MthdType im_head_look_MethType(char* recvBuf, int recvLen)
{
	int index = 0;
	IMPacketHeadPtr head  = (IMPacketHeadPtr)&recvBuf[4];

	if ( recvLen < IOTMAKERS_DEF_HEAD_LEN  )	{
		return MthdType_NONE;
	}

	return (MthdType)im_head_get_methodType(head);
}

/* =====================================
head -> extension
====================================== */
int im_pktHeadExt_get_count(IMPacketHeadPtr head)
{
	return head->ext_cnt;
}

int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val)
{
	IMHeadExtPtr ext = &(head->ext);
	if ( head->ext_cnt == 0 )	{
		im_headExt_init(ext);
	}

	if ( im_headExt_put_kvp_str(ext, key, val) < 0 )	{
		if ( head->ext_cnt == 0 ){
			im_headExt_release(ext);
		}
		return -1;
	}
	head->ext_cnt++;
	return 0;
}
char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key)
{
	IMHeadExtPtr ext = &(head->ext);
	if ( head->ext_cnt == 0 ){
		return NULL;
	}
	return (char*)im_headExt_get_kvp_str(ext, key);
}

/* =====================================
head -> extension -> (de)serialize
====================================== */
int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) 
{
	IMHeadExtPtr ext = &(head->ext);
	int index;
	int i;

	if ( head->ext_cnt <= 0 ) {
		return 0;
	}

	index = 0;

	for (i=0; i<head->ext_cnt; i++)
	{
		char *key = (char*)im_headExt_get_kvp_name_with_index(ext, i);
		if (key == NULL)	{
			break;
		}
		char *val = (char*)im_headExt_get_kvp_str(ext, key);

		// 1.key_len: 1BYTE
		unsigned char key_len = (unsigned char)strlen(key);
		key_len &= IOTMAKERS_8_BIT_MASKING;

		memcpy(&o_buff[index], &key_len, 1);
		index += 1;
		if ( index > o_buff_len ){
			ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 2. key: n BYTE
		memcpy(&o_buff[index], key, strlen(key));
		index += strlen(key);
		if ( index > o_buff_len ){
			ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 3. val_len: 2BYTE
		unsigned short val_len = (unsigned char)strlen(val);
		val_len = htons(val_len) & IOTMAKERS_16_BIT_MASKING;

		memcpy(&o_buff[index], &val_len, 2);
		index += 2;
		if ( index > o_buff_len ){
			ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 4. val: n BYTE
		memcpy(&o_buff[index], val, strlen(val));
		index += strlen(val);
		if ( index > o_buff_len ){
			ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}
	}

	return index;
}

int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) 
{
	int ext_len;
	int index;
	
	if ( im_head_get_hdrLen(head) <= IOTMAKERS_DEF_HEAD_LEN ) {
		return 0;
	}

	ext_len = im_head_get_hdrLen(head) - IOTMAKERS_DEF_HEAD_LEN;
	for (index = 0; index < ext_len; )
	{
		unsigned char key_len = 0;
		memcpy(&key_len, &buf[index], 1);
		index += 1;

		char *key = (char*)malloc(key_len+1);
		memcpy(key, &buf[index], key_len);
		key[key_len] = '\0';
		index += key_len;

		unsigned short val_len = 0;
		memcpy(&val_len, &buf[index], 2);
		val_len = ntohs(val_len) & IOTMAKERS_16_BIT_MASKING;
		index += 2;

		char *val = (char*)malloc(val_len+1);
		memcpy(val, &buf[index], val_len);
		val[val_len] = '\0';
		index += val_len;

		im_pktHeadExt_add_val(head, key, val);

		free(key);
		free(val);
	}

	return head->ext_cnt;
}


/* =====================================
 head (de)serialize
====================================== */
int im_head_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len)
{
	int hdlen = 0;
	int hdextlen = 0;

	hdlen = IOTMAKERS_DEF_HEAD_LEN;

	if ( o_buff_len < hdlen )	{
		ERR("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
		return -1;
	}

	if ( head->ext_cnt > 0 )	
	{
		hdextlen = im_pktHeadExt_get_serialized_to_buff(head, 
			&o_buff[IOTMAKERS_DEF_HEAD_LEN], o_buff_len-IOTMAKERS_DEF_HEAD_LEN);
		if ( hdextlen <= 0 ){
			ERR("fail im_pktHeadExt_get_serialized_to_buff()");
			return -1;
		}
		hdlen = IOTMAKERS_DEF_HEAD_LEN + hdextlen;
		im_head_set_hdrLen(head, hdlen);
	}

	memcpy(&o_buff[0], (char*)&(head->headRaw), IOTMAKERS_DEF_HEAD_LEN);
	
	return hdlen;
}


int im_head_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff)
{
	int hdlen = 0;
	int ext_cnt = 0;

	im_head_init(head);

	hdlen = IOTMAKERS_DEF_HEAD_LEN;
	memcpy((char*)&(head->headRaw), &i_buff[0], hdlen);

	if ( im_head_get_hdrLen(head) > hdlen ) {
		ext_cnt = im_pktHeadExt_get_deserialize_from_buff(head, &i_buff[hdlen]);
		if ( ext_cnt <= 0 ){
			ERR("fail im_pktHeadExt_get_deserialize_from_buff()");
			return -1;
		}
	}

	return im_head_get_hdrLen(head);
}
