/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; OTP확인 응답
 *
 *  @section 샘플패킷

data - serialized_string:

 *
 */
#include "packet/packet.h"


char* im_body731_res_get_otpRespCode(IMPacketBodyPtr body) 
{
	return jsonval_dotget_string(body->root, "otpRespCode");
}

char* im_body731_res_get_otpRespMessage(IMPacketBodyPtr body) 
{
	return jsonval_dotget_string(body->root, "otpRespMessage");
}

char* im_body731_res_get_phoneNumber(IMPacketBodyPtr body) 
{
	return jsonval_dotget_string(body->root, "phoneNumber");
}

