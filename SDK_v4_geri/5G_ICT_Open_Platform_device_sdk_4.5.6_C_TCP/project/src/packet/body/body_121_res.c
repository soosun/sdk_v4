/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 서버시간 조회 응답
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "srvrDt": "2017-08-23 15:56:20.345",
    "srvrTimeZone": "+09:00",
    "srvrDow": "4",
    "msgHeadVO": {
        "mapHeaderExtension": {}
    },
    "respCd": "100",
    "respMsg": "SUCCESS"
}
 *
 */
#include "packet/body/body.h"

char* im_body121_res_get_srvrDt(IMPacketBodyPtr body) 
{
	return jsonval_dotget_string(body->root, "srvrDt");
}

char* im_body121_res_get_srvrTimeZone(IMPacketBodyPtr body) 
{
	return jsonval_dotget_string(body->root, "srvrTimeZone");
}

