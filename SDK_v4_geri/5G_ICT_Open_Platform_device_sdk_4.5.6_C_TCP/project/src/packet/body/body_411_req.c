/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 데이터수집 요청
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "extrSysId": "extrSysId",
    "devColecDataVOs": [
        {
            "devId": "devId",
            "colecRowVOs": [
                {
					"occDt": "2014-02-28 17:59:17.517",
                    "snsnDataInfoVOs": [
                        {
                            "dataTypeCd": "aaa",
                            "snsnVal": 100
                        },
                        {
                            "dataTypeCd": "bbb",
                            "snsnVal": 100
                        },
                        {
                            "dataTypeCd": "ccc",
                            "snsnVal": 100
                        }
                    ],
                    "strDataInfoVOs": [
                        {
                            "snsnTagCd": "aaa",
                            "strVal": "xxx"
                        },
                        {
                            "snsnTagCd": "bbb",
                            "strVal": "yyy"
                        },
                        {
                            "snsnTagCd": "ccc",
                            "strVal": "zzz"
                        }
                    ]
                }
            ]
        }
    ]
}
 
 *
 */
#include "packet/body/body.h"



/** 
 *   @brief  im_body411_req_set_extrSysId  
        411Body 초기화; extrSysId(게이트웨이_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body411_req_set_extrSysId(IMPacketBodyPtr body, char *dev_gw)
{
  jsonval_dotset_string(body->root, "extrSysId", dev_gw);
  return 0;
}


/** 
 *   @brief  im_body411_req_append_devColecData  
        디바이스데이터 객체 추가; devId(디바이스_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body411_req_append_devColecData(IMPacketBodyPtr body, char* devId) 
{
  JSON_Value* devColecDataVO = bodyutil_init_devColecDataVO(devId); 
  jsonval_append_value_to_arrray(body->root, "devColecDataVOs", devColecDataVO);
  body->devColecDataIdx++;
  body->colecRowIdx = (-1);
	return 0;
}

/** 
 *   @brief  im_body411_req_append_colecRow  
        디바이스데이터 객체에 데이터셋 객체 추가
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body411_req_append_colecRow(IMPacketBodyPtr body, char *occDateStr, char *groupTagCd) 
{
  JSON_Value* devColecDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devColecDataVOs", body->devColecDataIdx); 
  JSON_Value* colecRowVO = bodyutil_init_colecRowVO(occDateStr, groupTagCd); 
  jsonval_append_value_to_arrray(devColecDataVO, "colecRowVOs", colecRowVO);
  body->colecRowIdx++;
	return 0;
}




static JSON_Value* __im_body411_req_get_cureent_colecRowVO(IMPacketBodyPtr body)
{
  JSON_Value* devColecDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devColecDataVOs", body->devColecDataIdx); 
  JSON_Value* colecRowVO = jsonval_dotget_value_on_arrray_with_index(devColecDataVO, "colecRowVOs", body->colecRowIdx); 
  return colecRowVO;
}


/** 
 *   @brief  im_body411_req_append_number_data  
        데이터셋 객체에 number데이터 추가
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body411_req_set_number_data(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* snsnDataInfoVO = bodyutil_init_snsnDataInfoVO(dataTypeCd, snsnVal); 
  bodyutil_append_snsnDataInfoVO_to_colecRowVO(colecRowVO, snsnDataInfoVO);
	return 0;
}


/** 
 *   @brief  im_body411_req_append_string_data  
        데이터셋 객체에 string데이터 추가
 *  
 *   @param  body is a pointer of IMPacketBodyPtr
 *   @return 0
 */  
int im_body411_req_set_string_data(IMPacketBodyPtr body, char* dataTypeCd, char* strVal) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* strDataInfoVO = bodyutil_init_strDataInfoVO(dataTypeCd, strVal); 
  bodyutil_append_strDataInfoVO_to_colecRowVO(colecRowVO, strDataInfoVO);
	return 0;
}


/**
new apis 2020-10-12 
*/
int im_body411_req_set_int_data(IMPacketBodyPtr body, char* dataTypeCd, int intVal) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_intDataInfoVO(dataTypeCd, intVal); 
  bodyutil_append_intDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}

int im_body411_req_set_realnum_data(IMPacketBodyPtr body, char* dataTypeCd, double realNum) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_rlNumDataInfoVO(dataTypeCd, realNum); 
  bodyutil_append_rlNumDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}

int im_body411_req_set_bool_data(IMPacketBodyPtr body, char* dataTypeCd, int boolVal) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_booleanDataInfoVO(dataTypeCd, boolVal); 
  bodyutil_append_booleanDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}

int im_body411_req_set_datetimestr_data(IMPacketBodyPtr body, char* dataTypeCd, char* strVal) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* strDataInfoVO = bodyutil_init_dtDataInfoVO(dataTypeCd, strVal); 
  bodyutil_append_dtDataInfoVO_to_colecRowVO(colecRowVO, strDataInfoVO);
	return 0;
}

int im_body411_req_set_datetime_data(IMPacketBodyPtr body, char* dataTypeCd, long sec) 
{
	char datetime_str[60];

	im_util_strftime_on_sec(sec, datetime_str, "%Y-%m-%d %H:%M:%S.000");

	return im_body411_req_set_datetimestr_data(body, dataTypeCd, datetime_str);
}








int im_body411_req_set_string_data_list(IMPacketBodyPtr body, char* dataTypeCd, int cnt, char** strValList) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataInfoVO = bodyutil_init_strDataInfoVO_list(dataTypeCd, cnt, strValList); 
  bodyutil_append_strListDataInfoVO_to_colecRowVO(colecRowVO, dataInfoVO);

	return 0;
}


int im_body411_req_set_int_data_list(IMPacketBodyPtr body, char* dataTypeCd, int cnt, double* valList) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataInfoVO = bodyutil_init_intListDataInfoVOs_list(dataTypeCd, cnt, valList); 
  bodyutil_append_intListDataInfoVO_to_colecRowVO(colecRowVO, dataInfoVO);

	return 0;
}


int im_body411_req_set_realnum_data_list(IMPacketBodyPtr body, char* dataTypeCd, int cnt, double* valList) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataInfoVO = bodyutil_init_rlNumListDataInfoVOs_list(dataTypeCd, cnt, valList); 
  bodyutil_append_rlNumListDataInfoVO_to_colecRowVO(colecRowVO, dataInfoVO);

	return 0;
}


int im_body411_req_set_bool_data_list(IMPacketBodyPtr body, char* dataTypeCd, int cnt, double* valList) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataInfoVO = bodyutil_init_booleanListDataInfoVOs_list(dataTypeCd, cnt, valList); 
  bodyutil_append_booleanListDataInfoVO_to_colecRowVO(colecRowVO, dataInfoVO);

	return 0;
}


int im_body411_req_set_string_datetimestr_list(IMPacketBodyPtr body, char* dataTypeCd, int cnt, char** strValList) 
{
  JSON_Value* colecRowVO = __im_body411_req_get_cureent_colecRowVO(body); 

  JSON_Value* dataInfoVO = bodyutil_init_dtListDataInfoVO_list(dataTypeCd, cnt, strValList); 
  bodyutil_append_dtListDataInfoVO_to_colecRowVO(colecRowVO, dataInfoVO);

	return 0;
}


