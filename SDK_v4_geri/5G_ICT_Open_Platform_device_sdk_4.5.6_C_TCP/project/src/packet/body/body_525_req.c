/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 제어데이터 요청
 *
 *  @section 샘플패킷

control data - serialized_string:
{
    "mapHeaderExtension": {},
    "devCnvyDataVOs": [
        {
            "devId": "river4D1450836201263",
            "cnvyRowVOs": [
                {
                    "snsnDataInfoVOs": [
                        {
                            "dataTypeCd": "controll01",
                            "snsnVal": 6
                        }
                    ],
                    "sttusDataInfoVOs": [],
                    "contlDataInfoVOs": [],
                    "cmdDataInfoVOs": [],
                    "binDataInfoVOs": [],
                    "strDataInfoVOs": [],
                    "dtDataInfoVOs": [],
                    "genlSetupDataInfoVOs": [],
                    "sclgSetupDataInfoVOs": [],
                    "binSetupDataInfoVOs": [],
                    "mapRowExtension": {}
                }
            ]
        }
    ],
    "msgHeadVO": {
        "mapHeaderExtension": {}
    }
}
 
 *
 */
#include "packet/body/body.h"

int im_body525_req_start_parse(IMPacketBodyPtr body) 
{
    // 데이터 핸들링을 위한 초기화
    body->devColecDataIdx = 0;
    body->colecRowIdx = 0;;
    return 0;
}


int im_body525_req_next_devColecDataIdx(IMPacketBodyPtr body) 
{
    body->devColecDataIdx++;
    body->colecRowIdx = 0;
    return 0;
}

int im_body525_req_next_colecRowIdx(IMPacketBodyPtr body) 
{
    body->colecRowIdx++;
    return 0;
}

int im_body525_req_get_numdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, double *o_val)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* snsnDataInfoVO = jsonval_dotget_value_on_arrray_with_index(cnvyRowVO, "snsnDataInfoVOs", idx);
	if ( snsnDataInfoVO == NULL )	{
		return -1;
	}

	*o_devid = (char*)jsonval_to_string(jsonval_dotget_value(devCnvyDataVO, "devId"));
	*o_tagid = (char*)jsonval_to_string(jsonval_dotget_value(snsnDataInfoVO, "dataTypeCd"));
	*o_val = (double)jsonval_to_number(jsonval_dotget_value(snsnDataInfoVO, "snsnVal"));

	return 0;
}




int im_body525_req_get_strdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, char **o_val)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jsonval_dotget_value_on_arrray_with_index(cnvyRowVO, "strDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}

	*o_devid = (char*)jsonval_to_string(jsonval_dotget_value(devCnvyDataVO, "devId"));
	*o_tagid = (char*)jsonval_to_string(jsonval_dotget_value(strDataInfoVO, "snsnTagCd"));
	*o_val = (char*)jsonval_to_string(jsonval_dotget_value(strDataInfoVO, "strVal"));

	return 0;
}


/**
added 2020-10-12
*/

int im_body525_req_get_groupTagCode (IMPacketBodyPtr body, char **o_groupTagCode)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	*o_groupTagCode = jsonval_dotget_string(cnvyRowVO, "groupTagCd");

	return 0;
}


int im_body525_req_get_integerdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, int *o_val)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jsonval_dotget_value_on_arrray_with_index(cnvyRowVO, "intDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}

	*o_devid = (char*)jsonval_to_string(jsonval_dotget_value(devCnvyDataVO, "devId"));
	*o_tagid = (char*)jsonval_to_string(jsonval_dotget_value(strDataInfoVO, "dataTypeCd"));
	*o_val = (int)jsonval_to_number(jsonval_dotget_value(strDataInfoVO, "intVal"));

	return 0;
}


int im_body525_req_get_floatdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, double *o_val)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jsonval_dotget_value_on_arrray_with_index(cnvyRowVO, "rlNumDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}

	*o_devid = (char*)jsonval_to_string(jsonval_dotget_value(devCnvyDataVO, "devId"));
	*o_tagid = (char*)jsonval_to_string(jsonval_dotget_value(strDataInfoVO, "dataTypeCd"));
	*o_val = (double)jsonval_to_number(jsonval_dotget_value(strDataInfoVO, "rlNumVal"));

	return 0;
}


int im_body525_req_get_booleandata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, int *o_val)
{
    if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jsonval_dotget_value_on_arrray_with_index(devCnvyDataVO, "cnvyRowVOs", body->colecRowIdx);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jsonval_dotget_value_on_arrray_with_index(cnvyRowVO, "booleanDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}

	*o_devid = (char*)jsonval_to_string(jsonval_dotget_value(devCnvyDataVO, "devId"));
	*o_tagid = (char*)jsonval_to_string(jsonval_dotget_value(strDataInfoVO, "dataTypeCd"));
	*o_val = (int)jsonval_to_number(jsonval_dotget_value(strDataInfoVO, "booleanVal"));

	return 0;
}
