#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include "led.h"
#include "pcf8574.h"
#include "adc.h"
#include "rtc.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4&F7������
//http ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/8/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 

#define NUM_CONFIG_CGI_URIS	2  //CGI��URI����
#define NUM_CONFIG_SSI_TAGS	4  //SSI��TAG����

//����LED��BEEP��CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* IP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

char IP_addr_cache[20];

static const char *ppcTAGs[]=  //SSI��Tag
{
	"0", //ADCֵ
	"1", //�¶�ֵ
	"2", //ʱ��
	"3"  //����
};

static const tCGI ppcURLs[]= //cgi����
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/ip.cgi",IP_CGI_Handler},
};


//��web�ͻ��������������ʱ��,ʹ�ô˺�����CGI handler����
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //����iLOOP
		}
	}
	return (-1);
}


void TEST_Handler(char *pcInsert)
{ 
	//׼����ӵ�html�е�����
    *pcInsert       = 'F';
    *(pcInsert + 1) = 'U';
    *(pcInsert + 2) = 'C';
    *(pcInsert + 3) = 'K';
}

void TEST1_Handler(char *pcInsert)
{      
    //׼����ӵ�html�е�����
	u8 ip_index = 0;
		for(ip_index = 0; ip_index < 20; ip_index++){
					*(pcInsert + ip_index) = IP_addr_cache[ip_index];
					if(!IP_addr_cache[ip_index]){
						break;
					}
				} 
}

static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 
				TEST_Handler(pcInsert);
				break;
		case 1: 
				TEST1_Handler(pcInsert);
				break;
		
	}
	return strlen(pcInsert);
}

//CGI LED���ƾ��
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //ע������Լ���GET�Ĳ����Ķ�����ѡ��iֵ��Χ
	iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //�ҵ�led��������
	//ֻ��һ��CGI��� iIndex=0
	if (iIndex != -1)
	{
		LED1(1);  //�ر�LED1��
		for (i=0; i<iNumParams; i++) //���CGI����
		{
		  if (strcmp(pcParam[i] , "LED1")==0)  //������"led" ���ڿ���LED1�Ƶ�
		  {
			if(strcmp(pcValue[i], "LED1ON") ==0)  //�ı�LED1״̬
				LED1(0); //��LED1
			else if(strcmp(pcValue[i],"LED1OFF") == 0)
				LED1(1); //�ر�LED1
		  }
		}
	 }
	return "/index.shtml";					
}


//CGI IP���ƾ��
const char* IP_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //ע������Լ���GET�Ĳ����Ķ�����ѡ��iֵ��Χ
	u8 ip_index = 0;
	iIndex = FindCGIParameter("IPaddr",pcParam,iNumParams);
	//ֻ��һ��CGI��� iIndex=0
	if (iIndex != -1)
	{		
		for (i=0; i<iNumParams; i++) //���CGI����
		{		  
			if (strcmp(pcParam[i] , "IPaddr")==0)  //������"led" ���ڿ���LED1�Ƶ�
		  {
				for(ip_index = 0; ip_index < 20; ip_index++){
					IP_addr_cache[ip_index] = pcValue[i][ip_index];
					if(!IP_addr_cache[ip_index]){
						break;
					}
				}
		  }
		}
	 }
	return "/index.shtml";					
}

//SSI�����ʼ��
void httpd_ssi_init(void)
{  
	//����SSI���
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI�����ʼ��
void httpd_cgi_init(void)
{ 
  //����CGI���
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


