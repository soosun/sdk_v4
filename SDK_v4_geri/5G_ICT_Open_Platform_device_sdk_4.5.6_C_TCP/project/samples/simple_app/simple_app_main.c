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

#include <stdio.h>
#include "client/client.h"
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>

#define LED1 5 // BCM_GPIO 24
#define TC1 4  // BCM_GPIO 23
#define MAXTIMINGS 85
#define DHTPIN 7
int dht11_dat[5] = {0, 0, 0, 0};

static char g_report_str[250];

char* example_if525_request_handler_for_string(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, char *prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%s]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");
	
	sprintf(g_report_str, "{\"%s\":\"%s\"}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_integer(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%d}", prop_name, prop_value);

	return prop_value;

}

double example_if525_request_handler_for_float(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, double prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%g]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%g}", prop_name, prop_value);

	return prop_value;
}

int example_if525_request_handler_for_boolean(im_client_tPtr cli, char *dev_id, char *resource_name, char *prop_name, int prop_value)
{
	printf("====================================\n");
	printf("= devid=[%s], resource_name=[%s], tagid=[%s], strval=[%d]\n", dev_id, resource_name, prop_name, prop_value);
	printf("====================================\n");

	sprintf(g_report_str, "{\"%s\":%s}", prop_name, (prop_value==1)?"true":"false");

	return prop_value;
}

int example_if525_handler_on_end_of_control(im_client_tPtr cli, char *dev_id, char *resource_name, void *report_body)
{
    printf("====================================\n");
    printf("= devid=[%s], resource_name=[%s] End of control\n", dev_id, resource_name);
    printf("= g_report_str=[%s]\n", g_report_str);
    printf("====================================\n");
    // 여기 디바이스 제어 코드를 작성하세요.
    // 제어로직은 최대한 빨리 끝내세요.

    // 응답용 제어Tag별 결과값을 세팅하는 예제
    if ( report_body != NULL )	
    {
	// Report in JSON String
	if ( strlen(g_report_str) > 0 )
	{
	    if(strstr(g_report_str, "LED01"))
	    {
		if(strstr(g_report_str, "ON"))
		{
		    pinMode(LED1, OUTPUT);
		    digitalWrite(LED1, 1); // On

		    printf("= LED ON - ok!!\n");
		    im_set_report_data_on_dev(report_body, g_report_str, resource_name, dev_id);
		    g_report_str[0] = '\0';
		    
		    return (100);
		}
		else if(strstr(g_report_str, "OFF"))
		{
		    pinMode(LED1, OUTPUT);
		    digitalWrite(LED1, 0); // On
		    
		    printf("= LED OFF - ok!!\n");
		    im_set_report_data_on_dev(report_body, g_report_str, resource_name, dev_id);
		    g_report_str[0] = '\0';

		    return (100);
		}
	    }
	    
	    im_set_report_data_on_dev(report_body, g_report_str, resource_name, dev_id);
	    g_report_str[0] = '\0';
	} 
	else
	{
	    char *json_str = "{\"command\":\"OFF\"}";
	    im_set_report_data_on_dev(report_body, json_str, resource_name, dev_id);
	}
    }

    // 응답용 제어결과 코드를 리턴
    return (200); // code: 100=OK, 200=ERR
}

/** 
 *  수집데이터 전송 예  
 */  
//static int example_send_data_in_json_string(im_client_tPtr cli)
static int  example_send_data_in_json_string(im_client_tPtr cli, char *resource_name, char *prop_name, char *prop_value, int int_value)
{
    char *json_str_buff = (char*)malloc(strlen(prop_name) * sizeof(char) + 101);

    char int_buff[100];
    sprintf(int_buff, "%d", int_value);
    
    strcpy(json_str_buff, "{"); 
    strcat(json_str_buff, "\"");
    strcat(json_str_buff, prop_name);
    strcat(json_str_buff, "\":");
    strcat(json_str_buff, (char *)int_buff);
    strcat(json_str_buff, "}");
        
    int rc = im_send_data(cli, json_str_buff, resource_name);
    if (rc < 0)  
    {
	ERR("fail im_send_data()");	
	free(json_str_buff);
		
	return -1;
    }

    printf("==>Send Data for Platform: %s, %s\n", json_str_buff, resource_name);
    free(json_str_buff);

    return 0;
}
//
//////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
    int rc = -1;

    /********************************************************
    *  IoTMakers 디바이스 클라이언트 핸들러
    ********************************************************/
    im_client_t client;
//  if (argc != 6)  
//  {
//    	printf("Usage : %s {ec_ip} {ec_port} {dev_id} {dev_pw} {svc_gw}\n", argv[0]);
//    	exit(0);
//  }
//  char *ec_ip = argv[1];
//  int ec_port = atoi(argv[2]);
//  char *dev_id = argv[3];
//  char *dev_pw = argv[4];
//  char *svc_gw = argv[5];

    char *ec_ip = "ec.geriopenlab.com";
    int   ec_port = 10020;
    char *dev_id = "soosunD1616459979816";
    char *dev_pw = "d1hh1pgm9";
    char *svc_gw = "OPEN_TCP_001_soosun";
  
    printf("connection : ec_ip=[%s] ec_port=[%d] dev_id=[%s] dev_pw=[%s] svc_gw=[%s]\n", ec_ip, ec_port, dev_id, dev_pw, svc_gw);
  
    /********************************************************
    * 로그레벨 설정
    *   0=NoLog, 1=Error, 2=Info, 3=Debug
    ********************************************************/
    im_client_set_loglevel(1);

    /********************************************************
    *  핸들러 초기화
    ********************************************************/
    rc = im_client_init(&client);
    if ( rc < 0 )  
    {
	return -1;
    }

    /********************************************************
    *  디바이스 제어수신 처리기 등록 (숫자형/문자형)
    ********************************************************/
    client.if525_handler_for_string = example_if525_request_handler_for_string;
    client.if525_handler_for_integer = example_if525_request_handler_for_integer;
    client.if525_handler_for_float = example_if525_request_handler_for_float;
    client.if525_handler_for_boolean = example_if525_request_handler_for_boolean;
    client.if525_handler_on_end_of_control = example_if525_handler_on_end_of_control;

    /********************************************************
    *  접속 종료시 이 지점부터 다시 시작
    ********************************************************/
_AGAIN:

    /********************************************************
    *   EC 접속 주소 지정 (TCP 표준 인터페이스)
    ********************************************************/
    rc = im_connect_to(&client, ec_ip, ec_port);

    /********************************************************
    *  디바이스 인증 요청
    ********************************************************/
    rc = im_auth_device(&client, dev_id, dev_pw, svc_gw);
    if ( rc < 0 )  
    {
	im_disconnect(&client);
	return -1;
    }
    
    // Raspberry pi wiringPiSetup...
    printf("dht11 Raspberry pi\n");
    if(wiringPiSetup() == -1) exit(1); 

    while ( 1==1 )  
    {
	//read read dht11 data /config setup...
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float f;
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	// pull pin down for 18 millisecond... 
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40);
	pinMode(DHTPIN, INPUT);

	// detect change and read data...
	for(i = 0; i < MAXTIMINGS; i++) 
	{
	    counter = 0;
	    while(digitalRead(DHTPIN) == laststate) 
	    {
		counter++;
		delayMicroseconds(1);
		if(counter == 255) break;
	    }
	    laststate = digitalRead(DHTPIN);
	    if(counter == 255) break; // if while breaked by timer, break for...
	    
	    // ignore first 3 transitions 
	    if((i >= 4) && (i % 2 == 0)) 
	    {
		dht11_dat[j / 8] <<= 1;
		
		if (counter > 50) 
		    dht11_dat[j / 8] |= 1;
		j++;
	    }
	}
 
	if((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xff)))
	{
//	    printf("Humidity = %d.%d %% Temperature = %d.%d *C \n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
    
	    /********************************************************
	    *  예제) 수집데이터 전송 
	    ********************************************************/
	    rc = example_send_data_in_json_string(&client, "Temp", "Temp01", "0", (int)dht11_dat[2]);
	    if ( rc < 0 )  
	    {
		printf("전송실패 000\n");
		break;
	    }
	}
		     
	// TC value send ...
	pinMode(TC1, INPUT); 
	if(digitalRead(TC1)==0)
	{
	    /********************************************************
	    *  예제) 수집데이터 전송 
	    ********************************************************/
	    rc = example_send_data_in_json_string(&client, "Touch", "Touch01", "0", 0);
	    if ( rc < 0 )  
	    {
		printf("전송실패 000\n");
		break;
	    }
	}
	else
	{
	    printf("Detected !!\n");
 
 	    /********************************************************
	    *  예제) 수집데이터 전송 
	    ********************************************************/
	    rc = example_send_data_in_json_string(&client, "Touch", "Touch01", "0", 1);
	    if ( rc < 0 )  
	    {
		printf("전송실패 000\n");
		break;
	    }
	}

	im_msleep(1000);

	/********************************************************
	*  송수신 이벤트 폴링
	********************************************************/
	rc = im_poll(&client);
	if ( rc < 0 )  
	{
	    break;
	}
    }

    /********************************************************
    *  접속 해제
    ********************************************************/
    im_disconnect(&client);
 
    /********************************************************
    *  3초 후에 재접속 시도
    ********************************************************/
    //sleep(3);
    //goto _AGAIN;

    return 0;
}
