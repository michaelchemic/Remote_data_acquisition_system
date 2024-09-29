#include "bc260y.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC260Y BC260Y_Status;
////�����Ƶ���Ԫ��,���豸���涼���Բ�ѯ��//////////////////////
#define ProductKey     "a1SZ2mtWIur"             //��ƷKEY
#define DeviceName          "m001"      //
#define DeviceSecret    "9dc54467d48a50867dd3a089ef078224"  //
#define PubTopic         "/sys/a1SZ2mtWIur/m001/thing/event/property/post"//��������
#define SubTopic        "/sys/a1NXoxeFw4c/mzh002/thing/service/property/set"//��������
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<255;i++)
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
		printf("AT+QMTDISC=0\r\n");
		delay_ms(300);
    printf("AT+CFUN=1\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
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
    Clear_Buffer();	
}




void BC260Y_ConTCP(void)
{
	printf("AT+QICLOSE=0\r\n");//�ر�socekt����
	delay_ms(300);
    Clear_Buffer();
    IWDG_Feed();//ι��
}
void BC260Y_CreateTCPSokcet(void)//����sokcet
{

    printf("AT+QIOPEN=1,0,\"TCP\",\"47.99.80.89\",14269,1234,1\r\n");//��������TCP,����IP�Լ��������˿ں��� ,����ֱ���³��ķ�ʽ
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//����Ƿ��½�ɹ�
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//����Ƿ��½�ɹ�
            delay_ms(100);
		}  
     Clear_Buffer();	
    
}
void BC260Y_Senddata(uint8_t *len,uint8_t *data)//�ַ�����ʽ
{
    printf("AT+QSOSEND=0,%s,%s\r\n",len,data);
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

void BC260Y_RECData(void)
{

  
}




void  MQTT_Init(void)
{
    printf("AT+QMTCFG=\"version\",0,1\r\n");//�豸�汾
    delay_ms(500);
	
    printf("AT+QMTOPEN=0,\"183.230.40.39\",6002\r\n");//ͨ��TCP��ʽȥ����MQTT������ 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//���·���״̬
    while(strx==NULL)
    {
      strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//ȷ�Ϸ���ֵ��ȷ
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"597952366\",\"323641\",\"1234567890\"\r\n");//ȥ��¼MQTT���������豸ID����ƷID,��Ȩ.�û�����ʵ����Ҫ���и���
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
}



//����ONENET��Ҫ�ύJSON����
u8 MqttOnenet_Savedata(u8 *t_payload,u8 temp,u8 humi)
{

		char json[]="{\"datastreams\":[{\"id\":\"temp\",\"datapoints\":[{\"value\":%d}]},{\"id\":\"humi\",\"datapoints\":[{\"value\":%d}]}]}";
    char t_json[200];
  //  int payload_len;
    unsigned short json_len;
	  sprintf(t_json, json, temp, humi);
  //  payload_len = 1 + 2 + strlen(t_json)/sizeof(char);
    json_len = strlen(t_json)/sizeof(char);
    //type
    t_payload[0] = '\x01';
    //length
    t_payload[1] = (json_len & 0xFF00) >> 8;
    t_payload[2] = json_len & 0xFF;
	//json
  	memcpy(t_payload+3, t_json, json_len);
    return json_len+3;
}
