#include "bc260y.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC260Y BC260Y_Status;
unsigned char socketnum=0;//��ǰ��socket����
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		IWDG_Feed();//ι��
	
}
void BC260Y_Init(void)
{
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		printf("AT+QICLOSE=0\r\n");
		delay_ms(300);
    printf("AT+CFUN=1\r\n");//������Ƶ
    delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
        printf("AT+CGATT=1\r\n");//�������磬PDP
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//��ѯ����״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//��1
        Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//��ȡ����״̬
            delay_ms(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
		}
		printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    delay_ms(300);
     strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//����CSQ
     Clear_Buffer();
}




void BC260Y_CreateUDPSokcet(void)//����sokcet
{

    printf("AT+QIOPEN=0,0,\"UDP\",\"47.92.146.210\",9999,1234,1\r\n");//��������UDP,����IP�Լ��������˿ں��� ,����ֱ���³��ķ�ʽ
    delay_ms(300);//UDP������ģʽ������ȥ����Ƿ��¼�������ɹ�
    Clear_Buffer();	
	  printf("AT+QICFG=\"dataformat\",0,0\r\n");//�ַ�ģʽ
	  delay_ms(300);//UDP������ģʽ������ȥ����Ƿ��¼�������ɹ�
    Clear_Buffer();	
    
}
void BC260Y_Senddata(uint8_t *len,uint8_t *data)//�ַ�����ʽ
{
    printf("AT+QISEND=0,%s,%s\r\n",len,data);
	  delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//����Ƿ��ͳɹ�
            delay_ms(100);
		}  
	  Uart1_SendStr(RxBuffer);
		RxCounter=0;
}

void BC260Y_Senddatahex(uint8_t *len,uint8_t *data)//����ʮ����������
{
    printf("AT+QISENDEX=0,%s,%s\r\n",len,data);
        delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//����Ƿ��ͳɹ�
            delay_ms(100);
		}  
     Clear_Buffer();	
}

void BC260Y_RECData()
{

    strx=strstr((const char*)RxBuffer,(const char*)"++QIURC:");//����+QIURC:���������յ�UDP���������ص�����
    if(strx)
    {
       

      
            Clear_Buffer();
                 
    }
}

