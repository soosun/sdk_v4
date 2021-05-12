/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 인증 응답
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "athnRqtNo": "XXXXX",
    "athnNo": "000000003B9ACFFB00000000XXXXXXXX",
    "respCd": "100",
    "respMsg": "SUCCESS"
}
 
 *
 */
#include "packet/packet.h"

char* im_body224_res_body_get_athnRqtNo(IMPacketBodyPtr body)
{
    return jsonval_dotget_string(body->root, "athnRqtNo");
}
char* im_body224_res_body_get_athnNo(IMPacketBodyPtr body)
{
    return jsonval_dotget_string(body->root, "athnNo");
}
