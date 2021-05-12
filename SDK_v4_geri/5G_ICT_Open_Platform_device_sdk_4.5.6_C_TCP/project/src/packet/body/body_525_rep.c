/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 제어데이터 처리결과
 *
 *  @section 샘플패킷

report data - serialized_string:
{
    "extrSysId": "OPEN_TCP_001PTL001_10XXXXXXXX",
    "devCnvyDataVOs": [
        {
            "devId": "XXXXX",
            "cnvyRowVOs": [
                {
                    "occDt": "2017-08-24 11:03:48.226",
                    "snsnDataInfoVOs": [{
                        "dataTypeCd": "10001003",
                        "snsnVal": 0.7
                    }],
                    "strDataInfoVOs": [
                        {
                            "snsnTagCd": "result",
                            "strVal": "OK"
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
 *   @brief  im_body525_rep_set_extrSysId  
        525Body 초기화; extrSysId(게이트웨이_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketPtr
 *   @return 0
 */  
int im_body525_rep_set_extrSysId(IMPacketBodyPtr body, char *dev_gw)
{
	jsonval_dotset_string(body->root, "extrSysId", dev_gw);
    return 0;
}


/** 
 *   @brief  im_body525_rep_append_devCnvyData  
        디바이스데이터 객체 추가; devId(디바이스_아이디) 필요
 *  
 *   @param  body is a pointer of IMPacketPtr
 *   @return 0
 */  
int im_body525_rep_append_devCnvyData(IMPacketBodyPtr body, char* devId) 
{
	JSON_Value* devColecDataVO = bodyutil_init_devColecDataVO(devId); 
	jsonval_append_value_to_arrray(body->root, "devCnvyDataVOs", devColecDataVO);
    body->devColecDataIdx++;
    body->colecRowIdx = (-1);
	return 0;
}

/** 
 *   @brief  im_body525_rep_append_cnvyRow  
        디바이스데이터 객체에 데이터셋 객체 추가
 *  
 *   @param  body is a pointer of IMPacketPtr
 *   @return 0
 */  
int im_body525_rep_append_cnvyRow(IMPacketBodyPtr body, char *occDateStr, char *groupTagCd) 
{
    JSON_Value* devColecDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx); 
    JSON_Value* colecRowVO = bodyutil_init_colecRowVO(occDateStr, groupTagCd); 
	jsonval_append_value_to_arrray(devColecDataVO, "cnvyRowVOs", colecRowVO);
    body->colecRowIdx++;
	return 0;
}


static JSON_Value* __im_body525_rep_get_cureent_colecRowVO(IMPacketBodyPtr body)
{
    JSON_Value* devColecDataVO = jsonval_dotget_value_on_arrray_with_index(body->root, "devCnvyDataVOs", body->devColecDataIdx); 
    JSON_Value* colecRowVO = jsonval_dotget_value_on_arrray_with_index(devColecDataVO, "cnvyRowVOs", body->colecRowIdx); 
    return colecRowVO;
}


/** 
 *   @brief  im_body525_rep_set_number_data  
        데이터셋 객체에 number데이터 추가
 *  
 *   @param  body is a pointer of IMPacketPtr
 *   @return 0
 */  
int im_body525_rep_set_number_data(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal) 
{
  JSON_Value* colecRowVO = __im_body525_rep_get_cureent_colecRowVO(body); 

  JSON_Value* snsnDataInfoVO = bodyutil_init_snsnDataInfoVO(dataTypeCd, snsnVal); 
  bodyutil_append_snsnDataInfoVO_to_colecRowVO(colecRowVO, snsnDataInfoVO);
	return 0;
}

/** 
 *   @brief  im_body525_rep_set_string_data  
        데이터셋 객체에 string데이터 추가
 *  
 *   @param  body is a pointer of IMPacketPtr
 *   @return 0
 */  
int im_body525_rep_set_string_data(IMPacketBodyPtr body, char* dataTypeCd, char* strVal)
{
  JSON_Value* colecRowVO = __im_body525_rep_get_cureent_colecRowVO(body); 

  JSON_Value* strDataInfoVO = bodyutil_init_strDataInfoVO(dataTypeCd, strVal); 
  bodyutil_append_strDataInfoVO_to_colecRowVO(colecRowVO, strDataInfoVO);
	return 0;
}

/**
new apis 2020-10-27
*/
int im_body525_rep_set_int_data(IMPacketBodyPtr body, char* dataTypeCd, int intVal) 
{
  JSON_Value* colecRowVO = __im_body525_rep_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_intDataInfoVO(dataTypeCd, intVal); 
  bodyutil_append_intDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}

int im_body525_rep_set_realnum_data(IMPacketBodyPtr body, char* dataTypeCd, double realNum) 
{
  JSON_Value* colecRowVO = __im_body525_rep_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_rlNumDataInfoVO(dataTypeCd, realNum); 
  bodyutil_append_rlNumDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}

int im_body525_rep_set_bool_data(IMPacketBodyPtr body, char* dataTypeCd, int boolVal) 
{
  JSON_Value* colecRowVO = __im_body525_rep_get_cureent_colecRowVO(body); 

  JSON_Value* dataVo = bodyutil_init_booleanDataInfoVO(dataTypeCd, boolVal); 
  bodyutil_append_booleanDataInfoVO_to_colecRowVO(colecRowVO, dataVo);
	return 0;
}
