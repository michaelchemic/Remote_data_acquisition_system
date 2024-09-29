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
�˹���ʵ���������Ʒ����ݣ��û���Ҫ������Ԫ������������һ�¡����ֺ��Լ����豸һ��
***/

//������NB�汾
 int main(void)
 {	
    u8 temp,humi;
	 	u8 data_len,send_jason[200];
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(9600);//����1��ʼ����������PC���д�ӡģ�鷵������
    uart2_init(9600);//�Խ�BC260Y���ڳ�ʼ��
    IWDG_Init(7,625);    //8Sһ��
 //   while(DHT11_Init())
     {}
    BC260Y_Init();//���豸��ʼ��
    MQTT_Init();
   while(1)
    {  
   //     DHT11_Read_Data(&temp,&humi);//��ȡ��ʪ������
			  temp=12;
			  humi=67;
	      printf("AT+QMTPUB=0,0,0,0,\"$dp\"\r\n");//������Ϣ
				delay_ms(300);
			  data_len=MqttOnenet_Savedata(send_jason,temp, humi);
			  Uart2_SendDATA(send_jason,data_len);//��jason���ݵ�ONENET
				while((USART2->SR&0X40)==0){}// 
				USART2->DR = (u8) 0x1A;
        delay_ms(500);
				Clear_Buffer(); 
				LED=!LED;
        IWDG_Feed();//ι��
    }	 
 }






