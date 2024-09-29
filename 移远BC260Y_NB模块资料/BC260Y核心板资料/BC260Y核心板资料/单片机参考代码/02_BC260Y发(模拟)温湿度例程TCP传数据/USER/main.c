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
BC260Y��BC26ָ��������ݣ�ʹ��BC26��������
***/

//������NB�汾
 int main(void)
 {	
    u8 temp,humi;
    u8 sendata[10];
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();        //��ʼ��LED�˿�
	  uart_init(9600);//������PC���д�ӡģ�鷵������
    uart2_init(9600);//����2��ʼ������BC260Y
    IWDG_Init(7,625);    //8Sһ��
  //   while(DHT11_Init())
     {}
    BC260Y_Init();//���豸��ʼ��
    BC260Y_CreateTCPSokcet();//����һ��SOCKET����
   while(1)
    {  
   //   DHT11_Read_Data(&temp,&humi);//��ȡ��ʪ������
			temp=23;
			humi=68;
			sendata[0]=temp/10+0x30;
			sendata[1]=temp%10+0x30;
			sendata[2]=humi/10+0x30;
			sendata[3]=humi%10+0x30;//ת���ַ���ʽ
			sendata[4]=0;
			BC260Y_Senddata("4",sendata);//�����ݣ������ַ�����ʽ���ͣ�Ҳ֧��ʮ������
 			BC260Y_RECData();//������ 
			delay_ms(500);
			LED=!LED;
			IWDG_Feed();//ι��
    }	 
 }






