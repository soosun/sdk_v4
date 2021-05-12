/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 최종값조회 요청
 *
 *  @section 샘플패킷

IF-711-req - serialized_string:
{
     "extrSysId": "HOME_IOT",
     "inclDevIds": ["W_085DDD27FB7507"],
     "excluDevIds": [],
     "cmdDataInfoVOs": [{
         "dataTypeCd": "2502",
         "cmdData": []
     }]
 } 
 



{
	"extrSysId": "DEV001",
	"inclDevIds": [
		"testbyw"
	],
	"groupTagCd": "testAllType",
	"msgHeadVO": {
		"mapHeaderExtension": {}
	}
}


 *
 */
#include "packet/body/body.h"

int im_body711_req_start_parse(IMPacketBodyPtr body) 
{
    // 데이터 핸들링을 위한 초기화
    return 0;
}

int im_body711_req_get_inclDevId_with_index (IMPacketBodyPtr body, int idx, char **o_inclDevId)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* inclDevId = jsonval_dotget_value_on_arrray_with_index(body->root, "inclDevIds", idx);
	if ( inclDevId == NULL )	{
		return -1;
	}

	*o_inclDevId = jsonval_to_string(inclDevId);

	return 0;
}


int im_body711_req_get_cmdDataInfo_with_index (IMPacketBodyPtr body, int idx, char **o_cmdType)
{
	if ( body->root == NULL ){
    return -1;
	}

	JSON_Value* cmdDataInfoVO = jsonval_dotget_value_on_arrray_with_index(body->root, "cmdDataInfoVOs", idx);
	if ( cmdDataInfoVO == NULL )	{
		return -1;
	}

	JSON_Value* dataTypeCd = jsonval_dotget_value(cmdDataInfoVO, "dataTypeCd");
	if ( dataTypeCd == NULL )	{
		return -1;
	}

	*o_cmdType = jsonval_to_string(dataTypeCd);

	return 0;
}


int im_body711_req_get_groupTagCode (IMPacketBodyPtr body, char **o_groupTagCode)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* jval = jsonval_dotget_value(body->root, "groupTagCd");
	if ( jval == NULL )	{
		*o_groupTagCode = NULL;
		return -1;
	}

	*o_groupTagCode = jsonval_to_string(jval);

	return 0;
}

