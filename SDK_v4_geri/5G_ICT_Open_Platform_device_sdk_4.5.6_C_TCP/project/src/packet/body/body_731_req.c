/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; OTP 확인요청
 *
 *  @section 샘플패킷

data - serialized_string:

 *
 */
#include "packet/body/body.h"


/** 
 *   @brief  im_body731_req_set_extrSysId  
        731Body 초기화; extrSysId(게이트웨이_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body731_req_set_extrSysId(IMPacketBodyPtr body, char* extrSysId) 
{
	jsonval_dotset_string(body->root, "extrSysId", extrSysId);
	return 0;
}

int im_body731_req_set_devId(IMPacketBodyPtr body, char* devId) 
{
	jsonval_dotset_string(body->root, "devId", devId);
	return 0;
}

int im_body731_req_set_seed(IMPacketBodyPtr body, char* seed) 
{
	jsonval_dotset_string(body->root, "seed", seed);
	return 0;
}

int im_body731_req_set_otp(IMPacketBodyPtr body, char* otp) 
{
	jsonval_dotset_string(body->root, "otp", otp);
	return 0;
}

