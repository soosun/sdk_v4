/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 제어데이터 응답
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "respCd": "100",
    "respMsg": "SUCCESS"
}
 
 *
 */
#include "packet/packet.h"


char* im_body525_res_set_respCd(IMPacketBodyPtr body, char *val) 
{
	jsonval_dotset_string(body->root, "respCd", val);
    return 0;
}

char* im_body525_res_set_respMsg(IMPacketBodyPtr body, char *val) 
{
	jsonval_dotset_string(body->root, "respMsg", val);
    return 0;
}

