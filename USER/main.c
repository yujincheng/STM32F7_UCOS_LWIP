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
 ALIENTEK ������STM32F7������ ����ʵ��10
 WebServerʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//KEY����
#define KEY_TASK_PRIO 		8
//�����ջ��С
#define KEY_STK_SIZE		128	
//�����ջ
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);   

//��LCD����ʾ��ַ��Ϣ����
//�������ȼ�
#define DISPLAY_TASK_PRIO	9
//�����ջ��С
#define DISPLAY_STK_SIZE	128
//�����ջ
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);


//LED����
//�������ȼ�
#define LED_TASK_PRIO		10
//�����ջ��С
#define LED_STK_SIZE		64
//�����ջ
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);  


//START����
//�������ȼ�
#define START_TASK_PRIO		11
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata); 

//��LCD����ʾ��ַ��Ϣ
//mode:1 ��ʾDHCP��ȡ���ĵ�ַ
//	  ���� ��ʾ��̬��ַ
//void show_address(u8 mode)
//{
//	u8 buf[30];
//	if(mode==2)
//	{
//		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
//		LCD_ShowString(30,130,210,16,16,buf); 
//		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
//		LCD_ShowString(30,150,210,16,16,buf); 
//		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
//		LCD_ShowString(30,170,210,16,16,buf); 
//	}
//	else 
//	{
//		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
//		LCD_ShowString(30,130,210,16,16,buf); 
//		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
//		LCD_ShowString(30,150,210,16,16,buf); 
//		sprintf((char*)buf,"NET MASK :%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
//		LCD_ShowString(30,170,210,16,16,buf); 
//	}	
//}

int main(void)
{
    Write_Through();                //����ǿ��͸д��
    MPU_Memory_Protection();        //������ش洢����
    Cache_Enable();                 //��L1-Cache
    
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
	usmart_dev.init(108); 		    //��ʼ��USMART
    LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
//    LCD_Init();                     //��ʼ��LCD
    PCF8574_Init();                 //��ʼ��PCF8574
    MY_ADC_Init();                  //��ʼ��ADC
    RTC_Init();                     //��ʼ��RTC 
    my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMDTCM);		    //��ʼ��DTCM�ڴ��
//	POINT_COLOR = RED; 		        //��ɫ����
//	LCD_ShowString(30,30,200,20,16,"Apollo STM32F4/F7");
//	LCD_ShowString(30,50,200,20,16,"TCP Server NETCONN Test");
//	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,90,200,20,16,"2016/2/24");
    
	OSInit(); 					    //UCOS��ʼ��
	while(lwip_comm_init()) 	    //lwip��ʼ��
	{
//		LCD_ShowString(30,110,200,20,16,"Lwip Init failed! "); 	//lwip��ʼ��ʧ��
		delay_ms(500);
//		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500);
	}
//	LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip��ʼ���ɹ�
    httpd_init();                                               //http��ʼ��
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //����UCOS						  	       
}

//start����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  			//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  	//���ж�
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //����DHCP����
#endif
	
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);//����LED����
//	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //��ʾ����
	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  		//���ж�
}

//��ʾ��ַ����Ϣ
//void display_task(void *pdata)
//{
//	while(1)
//	{ 
//#if LWIP_DHCP									//������DHCP��ʱ��
//		if(lwipdev.dhcpstatus != 0) 			//����DHCP
//		{
//			show_address(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
//			OSTaskSuspend(OS_PRIO_SELF); 		//��ʾ���ַ��Ϣ�������������
//		}
//#else
//		show_address(0); 						//��ʾ��̬��ַ
//		OSTaskSuspend(OS_PRIO_SELF); 		 	//��ʾ���ַ��Ϣ�������������
//#endif //LWIP_DHCP

//		OSTimeDlyHMSM(0,0,0,500);
//	}
//}




//led����
void led_task(void *pdata)
{
	while(1)
	{
		LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,500);  //��ʱ500ms
 	}
}
