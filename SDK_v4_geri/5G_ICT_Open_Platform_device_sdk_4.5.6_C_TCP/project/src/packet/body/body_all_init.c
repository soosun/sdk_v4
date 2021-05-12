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



#include "packet/body/body.h"

int im_body000_req_release(IMPacketBodyPtr body)
{
    im_body_release(body);
	return 0;
}

int im_body121_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}

int im_body224_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}

int im_body231_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}

int im_body332_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
    body->devBasIdx = (-1);;
	return 0;
}

int im_body411_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
    body->devColecDataIdx = (-1);
    body->colecRowIdx = (-1);;
	return 0;
}

int im_body525_res_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}

int im_body525_rep_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
    body->devColecDataIdx = (-1);
    body->colecRowIdx = (-1);;
	return 0;
}

int im_body711_res_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
    body->devColecDataIdx = (-1);
    body->colecRowIdx = (-1);;
	return 0;
}

int im_body731_req_init(IMPacketBodyPtr body){
  	im_body_init_with_string(body, "{}");
	return 0;
}




