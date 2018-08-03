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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4&F7开发板
//http 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/8/5
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 

#define NUM_CONFIG_CGI_URIS	2  //CGI的URI数量
#define NUM_CONFIG_SSI_TAGS	4  //SSI的TAG数量

//控制LED和BEEP的CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* IP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);

char IP_addr_cache[20];

static const char *ppcTAGs[]=  //SSI的Tag
{
	"0", //ADC值
	"1", //温度值
	"2", //时间
	"3"  //日期
};

static const tCGI ppcURLs[]= //cgi程序
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/ip.cgi",IP_CGI_Handler},
};


//当web客户端请求浏览器的时候,使用此函数被CGI handler调用
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //返回iLOOP
		}
	}
	return (-1);
}


void TEST_Handler(char *pcInsert)
{ 
	//准备添加到html中的数据
    *pcInsert       = 'F';
    *(pcInsert + 1) = 'U';
    *(pcInsert + 2) = 'C';
    *(pcInsert + 3) = 'K';
}

void TEST1_Handler(char *pcInsert)
{      
    //准备添加到html中的数据
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

//CGI LED控制句柄
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
	iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //找到led的索引号
	//只有一个CGI句柄 iIndex=0
	if (iIndex != -1)
	{
		LED1(1);  //关闭LED1灯
		for (i=0; i<iNumParams; i++) //检查CGI参数
		{
		  if (strcmp(pcParam[i] , "LED1")==0)  //检查参数"led" 属于控制LED1灯的
		  {
			if(strcmp(pcValue[i], "LED1ON") ==0)  //改变LED1状态
				LED1(0); //打开LED1
			else if(strcmp(pcValue[i],"LED1OFF") == 0)
				LED1(1); //关闭LED1
		  }
		}
	 }
	return "/index.shtml";					
}


//CGI IP控制句柄
const char* IP_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
	u8 ip_index = 0;
	iIndex = FindCGIParameter("IPaddr",pcParam,iNumParams);
	//只有一个CGI句柄 iIndex=0
	if (iIndex != -1)
	{		
		for (i=0; i<iNumParams; i++) //检查CGI参数
		{		  
			if (strcmp(pcParam[i] , "IPaddr")==0)  //检查参数"led" 属于控制LED1灯的
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

//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
  //配置CGI句柄
  http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}


