/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */


#ifndef IOTMAKERS_BODY_UTIL_H
#define IOTMAKERS_BODY_UTIL_H

#include "parson/parson.h"

#ifdef __cplusplus
extern "C"
{
#endif

JSON_Value* bodyutil_init_snsnDataInfoVO(char *dataTypeCd, double snsnVal);
JSON_Value* bodyutil_init_strDataInfoVO(char *dataTypeCd, char *strVal);
JSON_Value* bodyutil_init_binDataInfoVO(char *dataTypeCd, char *binVal, int binValLen);
JSON_Value* bodyutil_init_binSetupDataInfoVO(char *snsnTagCd, char *setupVal, int binValLen);

JSON_Value* bodyutil_init_colecRowVO(char *occDateStr, char *groupTagCd);
JSON_Value* bodyutil_append_strDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* strDataInfoVO );
JSON_Value* bodyutil_append_snsnDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* snsnDataInfoVO );
JSON_Value* bodyutil_append_binDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* binDataInfoVO );

JSON_Value* bodyutil_init_intDataInfoVO(char *dataTypeCd, int intVal);
JSON_Value* bodyutil_append_intDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject );
JSON_Value* bodyutil_init_rlNumDataInfoVO(char *dataTypeCd, double rlNumVal);
JSON_Value* bodyutil_append_rlNumDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject );
JSON_Value* bodyutil_init_booleanDataInfoVO(char *dataTypeCd, int booleanVal);
JSON_Value* bodyutil_append_booleanDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject );
JSON_Value* bodyutil_init_dtDataInfoVO(char *dataTypeCd, char *dtStrVal);
JSON_Value* bodyutil_append_dtDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject );

JSON_Value* bodyutil_init_devColecDataVO(char* devId);
JSON_Value* bodyutil_init_cnvyRowVO(char *occDateStr, char *groupTagCd);
JSON_Value* bodyutil_append_strDataInfoVO_to_cnvyRowVO(JSON_Value* cnvyRowVO, JSON_Value* strDataInfoVO );
JSON_Value* bodyutil_append_snsnDataInfoVO_to_cnvyRowVO(JSON_Value* cnvyRowVO, JSON_Value* snsnDataInfoVO );

JSON_Value* bodyutil_init_devCnvyDataVO(char* devId);
JSON_Value* bodyutil_append_colecRowVO_to_devCnvyDataVO(JSON_Value* devColecDataVO, JSON_Value* cnvyRowVO );

JSON_Value* bodyutil_init_devBasVO(char* extrSysId, char* devId);
JSON_Value* bodyutil_init_devDtlVO(char* atribNm, char* atribVal);


/* =====================================
debug
====================================== */
void bodyutil_JSON_Value_print(JSON_Value* jval);

#ifdef __cplusplus
}
#endif



#endif



