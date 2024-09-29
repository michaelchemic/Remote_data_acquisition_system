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
#include "timer.h"
#include "stm32f10x_tim.h"
#include "bc260y.h"	 
#include "dht11.h"
extern char  RxBuffer[100],RxCounter;


/***
此功能实现往阿里云发数据，用户需要更改三元素与推送主题一致。保持和自己的设备一样
***/

//适用于NB版本
 int main(void)
 {	
    u8 temp,humi;
	 	u8 data_len,send_jason[200];
    delay_init();	    	 //延时函数初始化	  
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();		  		//初始化与LED连接的硬件接口
    uart_init(9600);//串口1初始化，可连接PC进行打印模块返回数据
    uart2_init(9600);//对接BC260Y串口初始化
    IWDG_Init(7,625);    //8S一次
 //   while(DHT11_Init())
     {}
    BC260Y_Init();//对设备初始化
    MQTT_Init();
   while(1)
    {  
   //     DHT11_Read_Data(&temp,&humi);//读取温湿度数据
			  temp=12;
			  humi=67;
	      printf("AT+QMTPUB=0,0,0,0,\"$dp\"\r\n");//发布消息
				delay_ms(300);
			  data_len=MqttOnenet_Savedata(send_jason,temp, humi);
			  Uart2_SendDATA(send_jason,data_len);//发jason数据到ONENET
				while((USART2->SR&0X40)==0){}// 
				USART2->DR = (u8) 0x1A;
        delay_ms(500);
				Clear_Buffer(); 
				LED=!LED;
        IWDG_Feed();//喂狗
    }	 
 }






