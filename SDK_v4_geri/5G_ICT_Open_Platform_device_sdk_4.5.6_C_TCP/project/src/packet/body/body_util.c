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


#include <string.h>
#include <stdio.h>

#include "util/util.h"
#include "packet/body/body.h"


/**********************  "snsnDataInfoVOs"  *****************************
2020-10-12 Open Iotmakers Legacy

					"snsnDataInfoVOs": [
						{
							"dataTypeCd": "aaa",
							"snsnVal": 100
						}
					],

*/

JSON_Value* bodyutil_init_snsnDataInfoVO(char *dataTypeCd, double snsnVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	jsonval_dotset_number(json_val, "snsnVal", snsnVal);
	return json_val;
}

JSON_Value* bodyutil_append_snsnDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* snsnDataInfoVO )
{
	jsonval_append_value_to_arrray(colecRowVO, "snsnDataInfoVOs", snsnDataInfoVO);
	return colecRowVO;
}
/*
2020-10-12 Open Iotmakers Legacy
********/



/**********************  "strDataInfoVOs"  *****************************

					"strDataInfoVOs": [
						{
							"snsnTagCd ": "exampleStringTag",
							"strVal": "exampleValue1"
						}
					],

*/
JSON_Value* bodyutil_init_strDataInfoVO(char *dataTypeCd, char *strVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "snsnTagCd", dataTypeCd);
	jsonval_dotset_string(json_val, "strVal", strVal);
	return json_val;
}

JSON_Value* bodyutil_append_strDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "strDataInfoVOs", valueObject);
	return colecRowVO;
}


/**********************  ""  *****************************
					"intDataInfoVOs": [
						{
							"dataTypeCd": "exampleIntegerTag",
							"intVal": 5
						}
					],
*/
JSON_Value* bodyutil_init_intDataInfoVO(char *dataTypeCd, int intVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	jsonval_dotset_number(json_val, "intVal", (double)intVal);
	return json_val;
}

JSON_Value* bodyutil_append_intDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "intDataInfoVOs", valueObject);
	return colecRowVO;
}


/**********************  ""  *****************************
					"rlNumDataInfoVOs": [
						{
							"dataTypeCd": "exampleRealNumTag",
							"rlNumVal": 5.5
						}
					],
*/
JSON_Value* bodyutil_init_rlNumDataInfoVO(char *dataTypeCd, double rlNumVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	jsonval_dotset_number(json_val, "rlNumVal", rlNumVal);
	return json_val;
}

JSON_Value* bodyutil_append_rlNumDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "rlNumDataInfoVOs", valueObject);
	return colecRowVO;
}

/**********************  ""  *****************************
					"booleanDataInfoVOs": [
						{
							"dataTypeCd": "exampleBooleanTag",
							"booleanVal": true
						}
					],
*/
JSON_Value* bodyutil_init_booleanDataInfoVO(char *dataTypeCd, int booleanVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	jsonval_dotset_bool(json_val, "booleanVal", booleanVal);
	return json_val;
}

JSON_Value* bodyutil_append_booleanDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "booleanDataInfoVOs", valueObject);
	return colecRowVO;
}

/**********************  ""  *****************************
					"dtDataInfoVOs": [
						{
							"snsnTagCd": "exampleDateTag",
							"dtVal": "2020-10-08 12:00:00.000"
						}
					],
*/
JSON_Value* bodyutil_init_dtDataInfoVO(char *dataTypeCd, char *dtStrVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "snsnTagCd", dataTypeCd);
	jsonval_dotset_string(json_val, "dtVal", dtStrVal);
	return json_val;
}

JSON_Value* bodyutil_append_dtDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "dtDataInfoVOs", valueObject);
	return colecRowVO;
}


/* =========================================================  
	ARRAY Types
   ========================================================= */


/**********************  ""  *****************************

					"strListDataInfoVOs": [
						{
							"dataTypeCd": "exampleStringListTag",
							"strVals": [
								"exampleValue1",
								"exampleValue2"
							]
						}
					],

*/
JSON_Value* bodyutil_init_strDataInfoVO_list(char *dataTypeCd, int cnt, char** strValList)
{
	JSON_Value *json_val = json_value_init_object();
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	
	int i = 0;
	for ( i=0 ;i<cnt ; i++)
	{
		char *val = (char*)strValList[i];
		//printf("[%d]=[%s]\n", i, val);
	    JSON_Value *jsonval = json_value_init_string (val); 
		jsonval_append_value_to_arrray(json_val, "strVals", jsonval);
	}
	return json_val;
}
JSON_Value* bodyutil_append_strListDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "strListDataInfoVOs", valueObject);
	return colecRowVO;
}


/**********************  ""  *****************************

					"intListDataInfoVOs": [
						{
							"dataTypeCd": "exampleIntegerListTag",
							"intVals": [
								15,
								10
							]
						}
					],

*/
JSON_Value* bodyutil_init_intListDataInfoVOs_list(char *dataTypeCd, int cnt, double* valList)
{
	JSON_Value *json_val = json_value_init_object();
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	
	int i = 0;
	for ( i=0 ;i<cnt ; i++)
	{
	    JSON_Value *jsonval = json_value_init_number (valList[i]); 
		jsonval_append_value_to_arrray(json_val, "intVals", jsonval);
	}
	return json_val;
}
JSON_Value* bodyutil_append_intListDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "intListDataInfoVOs", valueObject);
	return colecRowVO;
}



/**********************  ""  *****************************

					"rlNumListDataInfoVOs": [
						{
							"dataTypeCd": "exampleRealNumListTag",
							"rlNumVals": [
								2.5,
								1.1
							]
						}
					],

*/
JSON_Value* bodyutil_init_rlNumListDataInfoVOs_list(char *dataTypeCd, int cnt, double* valList)
{
	JSON_Value *json_val = json_value_init_object();
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	
	int i = 0;
	for ( i=0 ;i<cnt ; i++)
	{
	    JSON_Value *jsonval = json_value_init_number (valList[i]); 
		jsonval_append_value_to_arrray(json_val, "rlNumVals", jsonval);
	}
	return json_val;
}
JSON_Value* bodyutil_append_rlNumListDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "rlNumListDataInfoVOs", valueObject);
	return colecRowVO;
}

/**********************  ""  *****************************

					"booleanListDataInfoVOs": [
						{
							"dataTypeCd": "exampleBooleanListTag",
							"booleanVals": [
								true,
								false
							]
						}
					],

*/
JSON_Value* bodyutil_init_booleanListDataInfoVOs_list(char *dataTypeCd, int cnt, double* valList)
{
	JSON_Value *json_val = json_value_init_object();
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	
	int i = 0;
	for ( i=0 ;i<cnt ; i++)
	{
	    JSON_Value *jsonval = json_value_init_boolean ((int)valList[i]); 
		jsonval_append_value_to_arrray(json_val, "booleanVals", jsonval);
	}
	return json_val;
}
JSON_Value* bodyutil_append_booleanListDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "booleanListDataInfoVOs", valueObject);
	return colecRowVO;
}


/**********************  ""  *****************************

					"dtListDataInfoVOs": [
						{
							"dataTypeCd": "exampleDateListTag",
							"dtVals": [
								"2020-10-08 12:00:00.000"
							]
						}
					],

*/
JSON_Value* bodyutil_init_dtListDataInfoVO_list(char *dataTypeCd, int cnt, char** valList)
{
	JSON_Value *json_val = json_value_init_object();
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	
	int i = 0;
	for ( i=0 ;i<cnt ; i++)
	{
		char *val = (char*)valList[i];
	    JSON_Value *jsonval = json_value_init_string (val); 
		jsonval_append_value_to_arrray(json_val, "dtVals", jsonval);
	}
	return json_val;
}
JSON_Value* bodyutil_append_dtListDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* valueObject )
{
	jsonval_append_value_to_arrray(colecRowVO, "dtListDataInfoVOs", valueObject);
	return colecRowVO;
}





/*=============================================*/



/**********************  "binDataInfoVOs"  *****************************
{
    "dataTypeCd": "aaa",
    "binData": [1,2,255, 0]
}
*/
JSON_Value* bodyutil_init_binDataInfoVO(char *dataTypeCd, char *binVal, int binValLen)
{
	int i;
	JSON_Value *json_val = json_value_init_object();
	JSON_Value *json_arr_val = json_value_init_array();
	
	jsonval_dotset_string(json_val, "dataTypeCd", dataTypeCd);
	jsonval_dotset_value(json_val, "binData", json_arr_val);

	JSON_Array  *json_arr  = json_array  (json_arr_val);
	for ( i=0 ; i<binValLen; i++)
	{
		unsigned char ch = binVal[i];
		json_array_append_number(json_arr, (double)ch);
	}
	
	return json_val;
}

/**********************  "binSetupDataInfoVOs"  *****************************
{
    "snsnTagCd": "82000001",
    "setupVal": [1,22,33,44]
}
*/
JSON_Value* bodyutil_init_binSetupDataInfoVO(char *snsnTagCd, char *setupVal, int binValLen)
{
	int i;
	JSON_Value *json_val = json_value_init_object();
	JSON_Value *json_arr_val = json_value_init_array();
	
	jsonval_dotset_string(json_val, "snsnTagCd", snsnTagCd);
	jsonval_dotset_value(json_val, "setupVal", json_arr_val);

	JSON_Array  *json_arr  = json_array  (json_arr_val);
	for ( i=0 ; i<binValLen; i++)
	{
		unsigned char ch = setupVal[i];
		json_array_append_number(json_arr, (double)ch);
	}
	
	return json_val;
}


/**********************  "colecRowVOs"  *****************************
 {
	"occDt": "2014-02-28 17:59:17.517",
     "snsnDataInfoVOs": [
         {}
     ],
     "strDataInfoVOs": [
         {}
     ],
     "binDataInfoVOs": [
         {}
     ]
 }
*/


JSON_Value* bodyutil_init_colecRowVO(char *occDateStr, char *groupTagCd)
{
	JSON_Value *json_val = json_value_init_object();
	
  if ( occDateStr != NULL )    {
    jsonval_dotset_string(json_val, "occDt", occDateStr);
  } else {
    char timebuff[64];
    im_util_strftime_now(timebuff, "%Y-%m-%d %H:%M:%S.");
    sprintf(timebuff+strlen(timebuff), "%03d", im_util_gettime_now_msec());
    jsonval_dotset_string(json_val, "occDt", timebuff);
  }

  if ( groupTagCd != NULL )    {
        jsonval_dotset_string(json_val, "groupTagCd", groupTagCd);
    }

	return json_val;
}


JSON_Value* bodyutil_append_binDataInfoVO_to_colecRowVO(JSON_Value* colecRowVO, JSON_Value* binDataInfoVO )
{
	jsonval_append_value_to_arrray(colecRowVO, "binDataInfoVOs", binDataInfoVO);
	return colecRowVO;
}

/**********************  "devColecDataVOs"  *****************************
{
    "devId": "devId",
    "colecRowVOs": [
        {}
    ]
}
*/
JSON_Value* bodyutil_init_devColecDataVO(char* devId)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "devId", devId);
	return json_val;
}





/**********************  "cnvyRowVOs"  *****************************
 {
	"occDt": "2014-02-28 17:59:17.517",
     "snsnDataInfoVOs": [
         {}
     ],
     "strDataInfoVOs": [
         {}
     ]
 }
*/

JSON_Value* bodyutil_init_cnvyRowVO(char *occDateStr, char *groupTagCd)
{
	return bodyutil_init_colecRowVO(occDateStr, groupTagCd);
}



/**********************  "devCnvyDataVOs"  *****************************
{
    "devId": "devId",
    "cnvyRowVOs": [
        {}
    ]
}
*/
JSON_Value* bodyutil_init_devCnvyDataVO(char* devId)
{
	return bodyutil_init_devColecDataVO(devId);
}

JSON_Value* bodyutil_append_colecRowVO_to_devCnvyDataVO(JSON_Value* devColecDataVO, JSON_Value* cnvyRowVO )
{
	jsonval_append_value_to_arrray(devColecDataVO, "cnvyRowVOs", cnvyRowVO);
	return devColecDataVO;
}



/**********************  "devBasVOs"  *****************************
{
	"devBasVOs": [{
		"extrSysId": "",
		"devId": "",
		"upExtrSysId": "",
		"upDevId": "",

		"devNm": "",
		"ipAdr": "",
		"sttusCd": "",
		"makrCd": "",
		"modelCd": "",
		"frmwrVerNo": "",

		"useYn": "",
		"delYn": "",
		"lastMtnDt": ""
	}]
}

{
    "devBasVOs": [{
        "extrSysId": "HOME_IOT",
        "devId": "W_085DDD531F7F0D",
        "devTrtSttusCd": "00",
        "upDevId": "C_085DDD531F7F",
        "modelNm": "018c00420005",
        "frmwrVerNo": "39.18",
        "athnFormlCd": "0004",
        "athnRqtNo": "F21898613CE9955CE3F0DD9DA30FF30C",
        "athnNo": "00000000-3B9B-840B-0000-000000000001",
        "devDtlVOs": [{
            "atribNm": "GEN_DEV_CLASS",
            "atribVal": "10"
        }, {
            "atribNm": "SPCF_DEV_CLASS",
            "atribVal": "01"
        }, {
            "atribNm": "HW_VER",
            "atribVal": "11"
        }, {
            "atribNm": "IST_ICON_TY",
            "atribVal": "0700"
        }, {
            "atribNm": "USER_ICON_TY",
            "atribVal": "0700"
        }],
        "binSetupDataInfoVOs": [{
            "snsnTagCd": "82000001",
            "setupVal": [1]
        }]
    }]
} 
*/


/*
{
    "extrSysId": "HOME_IOT",
    "devId": "W_085DDD531F7F0D"
}
*/
JSON_Value* bodyutil_init_devBasVO(char* extrSysId, char* devId)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "extrSysId", extrSysId);
	jsonval_dotset_string(json_val, "devId", devId);
	return json_val;
}

/*
{
    "atribNm": "IST_ICON_TY",
    "atribVal": "0700"
}
*/
JSON_Value* bodyutil_init_devDtlVO(char* atribNm, char* atribVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jsonval_dotset_string(json_val, "atribNm", atribNm);
	jsonval_dotset_string(json_val, "atribVal", atribVal);
	return json_val;
}


/* =====================================
debug
====================================== */
void bodyutil_JSON_Value_print(JSON_Value* jval)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(jval);
	printf("jval:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
