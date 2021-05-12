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



#ifndef IOTMAKERS_PKTHEAD_H
#define IOTMAKERS_PKTHEAD_H

//#include "platform.h"
#include "packet/head/head_ext.h"

#define IOTMAKERS_PROTOCOL_VER_MAJOR		1
#define IOTMAKERS_PROTOCOL_VER_MINOR		1

#define IOTMAKERS_DEF_HEAD_LEN			35

#define IOTMAKERS_8_BIT_MASKING				0x000000FF
#define IOTMAKERS_16_BIT_MASKING			0x0000FFFF


/*
 * �޽������VO(IMPacketHead)�� �����Ѵ�.
 * �޽�������� ��� �޽���(��û/����/����)�� ���������� ���ԵǴ� �⺻ �Ӽ��̴�.
 *
 * KTǥ������������ ������ [TCP Packet Length (4 Byte)] + [Message Header(35 Byte)] + [Message Body]�� �����Ǵµ�,
 * �޽������VO�� [Message Header]�� �ǹ��Ѵ�.
 *
 * �׸��� �޽������VO�� ���۵� ��, 35 Byte�� ��ȯ�Ǿ� ���۵ǰ�, (�޽������Ȯ��VO �����ϰ�, 35 Byte)

 0000: 00 00 00 87 11 01 00 23 | 60 e0 24 12 62 57 00 00 
 ..................==.==.=====...-====.,,,,,,,,,,,,,,,,,
 ................................msgType:2--MsgType_REQUEST(1) 01
 ................................msgExchPtrn:2--MsgExchPtrn_ONE_WAY_ACK(2) 10
 ................................methodType:12--MthdType_ATHN_COMMCHATHN_DEV_TCP(224)
 ................................01100000 1110000

60e0 :x86 0xFF0F (OK)
60e0 :mips(R) 0x0FFF (OK)
 */

#pragma pack(1)
typedef struct {
	unsigned char			protoMainVer:4;			// �������� ��������		(4bit)
	unsigned char			protoSubVer:4;			// �������� ��������		(4bit)
	unsigned char			hdrType;				// ���Ÿ��					(8bit)
	unsigned short			hdrLen;					// ��� ����				(16bit)
	// 4 bytes

	union ugly {
		unsigned char __pad [2];
#if defined(REVERSED)
		struct ugly2 {
			unsigned char	msgType:2;				// �޽��� ����				(�ֻ���2bit)
			unsigned char	msgExchPtrn:2;			// �޽��� ��ȯ ����			(����2bit)
			unsigned char	__dummy:4;
		}msgInfo;
#else
		struct ugly2 {
			unsigned char	__dummy:4;
			unsigned char	msgExchPtrn:2;			// �޽��� ��ȯ ����			(����2bit)
			unsigned char	msgType:2;				// �޽��� ����				(�ֻ���2bit)
		}msgInfo;
#endif
		unsigned short		methodType;				// �������					(����12bit)
	}ugly;
	// 2 bytes

	unsigned long long		trmTransacId;			// Ʈ����Ǿ��̵�			(64bit)
	// 8 bytes

	char					commChAthnNo[16];		// ���ä��������ū	(128bit)
	// 16 bytes

	unsigned char			encryType:7;			// ��ȣȭ����				(����7bit)
	unsigned char			encryUsage:1;			// ��ȣȭ��뿩��			(����1bit)
	// 1 byte
	
	unsigned char			compType:7;				// ��������					(����7bit)
	unsigned char			compUsage:1;			// �����뿩��				(����1bit)
	// 1 byte
	
	unsigned char			encodType;				// ���ڵ�����				(8bit)
	// 1 byte
	
	unsigned short			rsltCd;					// ����ڵ�					(16bit)
	// 2 bytes
} IMHeadRaw, *IMHeadRawPtr;


#pragma pack(0)
typedef struct {
	IMHeadRaw	headRaw;
	IMHeadExt	ext;
	int			ext_cnt;	// ���Ȯ���ʵ��

} IMPacketHead, *IMPacketHeadPtr;


typedef enum {
	HdrType_BASIC				= 1
	, HdrType_LIGHT_WEIGHT		= 2
} HdrType;

typedef enum {
	MsgType_NONE				= 0
	, MsgType_REQUEST			= 1
	, MsgType_RESPONSE			= 2
	, MsgType_REPORT			= 3
} MsgType;

typedef enum {
	MsgExchPtrn_NONE			= 0
	, MsgExchPtrn_ONE_WAY		= 1
	, MsgExchPtrn_ONE_WAY_ACK	= 2
	, MsgExchPtrn_THREE_WAY		= 3
} MsgExchPtrn;

typedef enum {
	UseYn_NO					= 0
	, UseYn_YES					= 1
} UseYn;

typedef enum {
	EncdngType_USER_DEFINED		= 0x01
	, EncdngType_XML			= 0x02
	, EncdngType_JSON			= 0x03
	, EncdngType_GPB			= 0x10
	, EncdngType_THRIFT			= 0x11
	, EncdngType_AVRO			= 0x12
	, EncdngType_PCRE			= 0x13
} EncdngType;

typedef enum {
	EcodType_AES_128			= 0x01
	, EcodTypeAES_256			= 0x02
	, EcodType_DES_64			= 0x11
	, EcodType_DES_EDE_192		= 0x12
	, EcodType_SEED_128			= 0x21
	, EcodType_ARIA_128			= 0x31
	, EcodType_ARIA_192			= 0x32
	, EcodType_ARIA_256			= 0x33
} EcodType;

typedef enum {
	CmpreType_HUFFMAN = 1
	, CmpreType_RUN_LENGTH = 2
	, CmpreType_SHANNON_FANO = 3
} CmpreType;


typedef enum {
	MthdType_NONE							= 0

	/** ���� ����ð� ��ȸ */
	, MthdType_SVR_DT_REQ				    = 121
	
  /** ��ġ TCP ä�� ���� */
	, MthdType_ATHN_COMMCHATHN_DEV_TCP 		= 224
	
	/** KeepAlive TCP ä�� */
	, MthdType_KEEP_ALIVE_COMMCHATHN_TCP	= 231
	
	/** ��ġ���� ���ź��� */
	, MthdType_INITA_DEV_UDATERPRT			= 332
    
  /** ���� ���յ����� */
	, MthdType_COLEC_ITGDATA_RECV			= 411
  
	/** ���յ����� ���� */
	, MthdType_CONTL_ITGCNVY_DATA			= 525
 
	/** ������ ��ȸ */
	, MthdType_LAST_VAL_QUERY				= 711

	/** OTP Ȯ�� */
	, MthdType_OPT_CHECK_QUERY				= 731

} MthdType;

typedef enum {
	RespCd_NONE = 0

	/** ���� */
	, RespCd_NORMALITY = 100
	
	/** �Ϲݿ��� */
	, RespCd_GENERAL_ERROR = 200
	
	/** �������� */
	, RespCd_IMPLEMENTATION_ERROR = 201
	
	/** ��Ŷ Ǫ�� ���� */
	, RespCd_PACKET_PUSH_ERROR = 202
	
	/** ��ȣȭ ���� */
	, RespCd_DECRYPTION_ERROR = 203
	
	/** ��Ŷ�Ľ� ���� */
	, RespCd_PASSING_ERROR = 204
	
	/** ���� ���� */
	, RespCd_CERTIFICATION_ERROR = 205
	
	/** ���� ���� */
	, RespCd_ACK_ERROR = 206
	
	/** ���ä���������� */
	, RespCd_COMM_CH_ATHN_ERROR = 207
	
	/** ��û�������� */
	, RespCd_REQUEST_INFO_ERROR = 208
} RespCd;



#ifdef __cplusplus
extern "C"
{
#endif

// head.c 
void im_head_init(IMPacketHeadPtr head);
void im_head_init_oneway_req(IMPacketHeadPtr head);
void im_head_init_response(IMPacketHeadPtr head);
void im_head_init_report(IMPacketHeadPtr head);
void im_head_release(IMPacketHeadPtr head);
void im_head_copy_trmTransacId(IMPacketHeadPtr headDest, IMPacketHeadPtr headSrc);
void im_head_set_msgType(IMPacketHeadPtr head, MsgType type);
MsgType im_head_get_msgType(IMPacketHeadPtr head);
void im_head_set_msgExchPtrn(IMPacketHeadPtr head, MsgExchPtrn type);
MsgExchPtrn im_head_get_msgExchPtrn(IMPacketHeadPtr head);
void im_head_set_methodType(IMPacketHeadPtr head, MthdType type);
MthdType im_head_get_methodType(IMPacketHeadPtr head);
void im_head_set_trmTransacId(IMPacketHeadPtr head, unsigned long long trxid);
unsigned long long im_head_get_trmTransacId(IMPacketHeadPtr head);
void im_head_set_commChAthnNo(IMPacketHeadPtr head, char *authCode);
char* im_head_get_commChAthnNo(IMPacketHeadPtr head);
void im_head_set_respCd(IMPacketHeadPtr head, RespCd rcode);
RespCd im_head_get_respCd(IMPacketHeadPtr head);
void im_head_set_hdrLen(IMPacketHeadPtr head, unsigned short len);
unsigned short im_head_get_hdrLen(IMPacketHeadPtr head);
MthdType im_head_look_MethType(char* recvBuf, int recvLen);
int im_pktHeadExt_get_count(IMPacketHeadPtr head);
int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val);
char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key);
int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) ;
int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) ;
int im_head_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len);
int im_head_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff);


// head_all_init.c
int im_head000_req_release(IMPacketHeadPtr head);
int im_head121_req_init(IMPacketHeadPtr head);
int im_head224_req_init(IMPacketHeadPtr head );
int im_head231_req_init(IMPacketHeadPtr head, char *auth_code );
int im_head332_req_init(IMPacketHeadPtr head, char *auth_code );
int im_head411_req_init(IMPacketHeadPtr head, char *auth_code );
int im_head525_res_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid );
int im_head525_rep_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid );
int im_head711_res_init(IMPacketHeadPtr head, char *auth_code, unsigned long long txid );
int im_head731_req_init(IMPacketHeadPtr head, char *auth_code );

// head_ext.c
int im_headExt_init(IMHeadExtPtr pHExt) ;
int im_headExt_init_with_string(IMHeadExtPtr pHExt, char *json_str) ;
void im_headExt_release(IMHeadExtPtr pHExt) ;
int im_headExt_put_kvp_str(IMHeadExtPtr pHExt, char *key, char *val) ;
char* im_headExt_get_kvp_str(IMHeadExtPtr pHExt, char *key) ;
char* im_headExt_get_kvp_name_with_index(IMHeadExtPtr pHExt, int idx) ;
int im_packet_head_ext_get_serialized_strlen(IMHeadExtPtr pHExt) ;
char* im_packet_head_ext_get_serialized_str(IMHeadExtPtr pHExt) ;
void jsonwrap_print_value_serialized(JSON_Value* json_val);


#ifdef __cplusplus
}
#endif






#endif

