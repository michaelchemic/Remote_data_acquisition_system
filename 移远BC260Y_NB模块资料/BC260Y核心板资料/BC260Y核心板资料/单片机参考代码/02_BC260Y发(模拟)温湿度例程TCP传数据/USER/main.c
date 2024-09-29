#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "math.h"			
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "bc260y.h"	 
#include "dht11.h"
extern char  RxBuffer[100],RxCounter;
extern unsigned char uart1_getok;
extern char RxCounter1,RxBuffer1[100];
extern unsigned char Timeout,restflag;



/***
BC260Y与BC26指令基本兼容，使用BC26驱动开发
***/

//适用于NB版本
 int main(void)
 {	
    u8 temp,humi;
    u8 sendata[10];
    delay_init();	    	 //延时函数初始化	  
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();        //初始化LED端口
	  uart_init(9600);//可连接PC进行打印模块返回数据
    uart2_init(9600);//串口2初始化，与BC260Y
    IWDG_Init(7,625);    //8S一次
  //   while(DHT11_Init())
     {}
    BC260Y_Init();//对设备初始化
    BC260Y_CreateTCPSokcet();//创建一个SOCKET连接
   while(1)
    {  
   //   DHT11_Read_Data(&temp,&humi);//读取温湿度数据
			temp=23;
			humi=68;
			sendata[0]=temp/10+0x30;
			sendata[1]=temp%10+0x30;
			sendata[2]=humi/10+0x30;
			sendata[3]=humi%10+0x30;//转成字符形式
			sendata[4]=0;
			BC260Y_Senddata("4",sendata);//发数据，按照字符串方式发送，也支持十六进制
 			BC260Y_RECData();//收数据 
			delay_ms(500);
			LED=!LED;
			IWDG_Feed();//喂狗
    }	 
 }






