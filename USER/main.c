#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
//#include "lcd.h"
#include "sdram.h"
#include "lan8720.h"
#include "pcf8574.h"
#include "mpu.h"
#include "adc.h"
#include "rtc.h"
#include "usmart.h"
#include "malloc.h"
#include "malloc.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "includes.h"
#include "httpd.h"
#include "stmflash.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 网络实验10
 WebServer实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//KEY任务
#define KEY_TASK_PRIO 		8
//任务堆栈大小
#define KEY_STK_SIZE		128	
//任务堆栈
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);   

//在LCD上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	9
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务堆栈
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);


//LED任务
//任务优先级
#define LED_TASK_PRIO		10
//任务堆栈大小
#define LED_STK_SIZE		64
//任务堆栈
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);  


//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 

//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
//void show_address(u8 mode)
//{
//	u8 buf[30];
//	if(mode==2)
//	{
//		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
//		LCD_ShowString(30,130,210,16,16,buf); 
//		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
//		LCD_ShowString(30,150,210,16,16,buf); 
//		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
//		LCD_ShowString(30,170,210,16,16,buf); 
//	}
//	else 
//	{
//		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
//		LCD_ShowString(30,130,210,16,16,buf); 
//		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
//		LCD_ShowString(30,150,210,16,16,buf); 
//		sprintf((char*)buf,"NET MASK :%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
//		LCD_ShowString(30,170,210,16,16,buf); 
//	}	
//}

int main(void)
{
    Write_Through();                //开启强制透写！
    MPU_Memory_Protection();        //保护相关存储区域
    Cache_Enable();                 //打开L1-Cache
    
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
	usmart_dev.init(108); 		    //初始化USMART
    LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
//    LCD_Init();                     //初始化LCD
    PCF8574_Init();                 //初始化PCF8574
    MY_ADC_Init();                  //初始化ADC
    RTC_Init();                     //初始化RTC 
    my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
//	POINT_COLOR = RED; 		        //红色字体
//	LCD_ShowString(30,30,200,20,16,"Apollo STM32F4/F7");
//	LCD_ShowString(30,50,200,20,16,"TCP Server NETCONN Test");
//	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,90,200,20,16,"2016/2/24");
    
	OSInit(); 					    //UCOS初始化
	while(lwip_comm_init()) 	    //lwip初始化
	{
//		LCD_ShowString(30,110,200,20,16,"Lwip Init failed! "); 	//lwip初始化失败
		delay_ms(500);
//		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500);
	}
//	LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip初始化成功
    httpd_init();                                               //http初始化
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //开启UCOS						  	       
}

//start任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  			//初始化统计任务
	OS_ENTER_CRITICAL();  	//关中断
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //创建DHCP任务
#endif
	
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);//创建LED任务
//	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //显示任务
	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  		//开中断
}

//显示地址等信息
//void display_task(void *pdata)
//{
//	while(1)
//	{ 
//#if LWIP_DHCP									//当开启DHCP的时候
//		if(lwipdev.dhcpstatus != 0) 			//开启DHCP
//		{
//			show_address(lwipdev.dhcpstatus );	//显示地址信息
//			OSTaskSuspend(OS_PRIO_SELF); 		//显示完地址信息后挂起自身任务
//		}
//#else
//		show_address(0); 						//显示静态地址
//		OSTaskSuspend(OS_PRIO_SELF); 		 	//显示完地址信息后挂起自身任务
//#endif //LWIP_DHCP

//		OSTimeDlyHMSM(0,0,0,500);
//	}
//}




//led任务
void led_task(void *pdata)
{
	while(1)
	{
		LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,500);  //延时500ms
 	}
}
