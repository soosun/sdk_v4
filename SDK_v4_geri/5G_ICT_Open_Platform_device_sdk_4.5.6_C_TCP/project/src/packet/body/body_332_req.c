/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 장치정보 CRUD 요청
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "infoUpdTypeCd": "01",
    "devBasVOs": [
        {
            "extrSysId": "OPEN_TCP_001PTL001_10XXXXXXXX",
            "devId": "XXXXX",
            "TEST": "11223344"
        }
    ]
} 

 *
 */
#include "packet/body/body.h"



/** 
 *   @brief  im_body332_req_set_infoUpdTypeCd_insert  
        332Body 초기화; extrSysId(게이트웨이_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  

int im_body332_req_set_infoUpdTypeCd_insert(IMPacketBodyPtr body) 
{
	jsonval_dotset_string(body->root, "infoUpdTypeCd", "01");
    return 0;
}

int im_body332_req_set_infoUpdTypeCd_delete(IMPacketBodyPtr body) 
{
	jsonval_dotset_string(body->root, "infoUpdTypeCd", "21");
    return 0;
}

int im_body332_req_set_infoUpdTypeCd_update_all(IMPacketBodyPtr body) 
{
	jsonval_dotset_string(body->root, "infoUpdTypeCd", "11");
    return 0;
}

int im_body332_req_set_infoUpdTypeCd_update(IMPacketBodyPtr body) 
{
	jsonval_dotset_string(body->root, "infoUpdTypeCd", "13");
    return 0;
}

int im_body332_req_append_devBas(IMPacketBodyPtr body, char *extrSysId, char *devId)
{
    JSON_Value *devBasVO = bodyutil_init_devBasVO(extrSysId, devId);
	jsonval_append_value_to_arrray(body->root, "devBasVOs", devBasVO);
    body->devBasIdx++;
    return 0;
}

int im_body332_req_set_string_data (IMPacketBodyPtr body, char *attr, char *string)
{
	JSON_Value *devBasVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devBasVOs", body->devBasIdx);
	if ( devBasVO == NULL )	{
		return -1;
	}

	if ( attr != NULL && string != NULL) jsonval_dotset_string(devBasVO, attr, string);
	return 0;
}

int im_body332_req_set_number_data (IMPacketBodyPtr body, char *attr, double number)
{
	JSON_Value *devBasVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devBasVOs", body->devBasIdx);
	if ( devBasVO == NULL )	{
		return -1;
	}

	if ( attr != NULL ) jsonval_dotset_number(devBasVO, attr, number);
	return 0;
}

