#include "bc260y.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC260Y BC260Y_Status;
////阿里云的三元素,在设备里面都可以查询到//////////////////////
#define ProductKey     "a1SZ2mtWIur"             //产品KEY
#define DeviceName          "m001"      //
#define DeviceSecret    "9dc54467d48a50867dd3a089ef078224"  //
#define PubTopic         "/sys/a1SZ2mtWIur/m001/thing/event/property/post"//发布主题
#define SubTopic        "/sys/a1NXoxeFw4c/mzh002/thing/service/property/set"//订阅主题
void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<255;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
	
}
void BC260Y_Init(void)
{
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
		printf("AT+QMTDISC=0\r\n");
		delay_ms(300);
    printf("AT+CFUN=1\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
        printf("AT+CGATT=1\r\n");//激活网络，PDP
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//查询激活状态
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1
        Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//获取激活状态
            delay_ms(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
		}
		printf("AT+CSQ\r\n");//查看获取CSQ值
    delay_ms(300);
    Clear_Buffer();	
}




void BC260Y_ConTCP(void)
{
	printf("AT+QICLOSE=0\r\n");//关闭socekt连接
	delay_ms(300);
    Clear_Buffer();
    IWDG_Feed();//喂狗
}
void BC260Y_CreateTCPSokcet(void)//创建sokcet
{

    printf("AT+QIOPEN=1,0,\"TCP\",\"47.99.80.89\",14269,1234,1\r\n");//创建连接TCP,输入IP以及服务器端口号码 ,采用直接吐出的方式
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
            delay_ms(100);
		}  
     Clear_Buffer();	
    
}
void BC260Y_Senddata(uint8_t *len,uint8_t *data)//字符串形式
{
    printf("AT+QSOSEND=0,%s,%s\r\n",len,data);
}

void BC260Y_Senddatahex(uint8_t *len,uint8_t *data)//发送十六进制数据
{
    printf("AT+QISENDEX=0,%s,%s\r\n",len,data);
        delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//检查是否发送成功
            delay_ms(100);
		}  
     Clear_Buffer();	
}

void BC260Y_RECData(void)
{

  
}




void  MQTT_Init(void)
{
    printf("AT+QMTCFG=\"version\",0,1\r\n");//设备版本
    delay_ms(500);
	
    printf("AT+QMTOPEN=0,\"183.230.40.39\",6002\r\n");//通过TCP方式去连接MQTT服务器 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//看下返回状态
    while(strx==NULL)
    {
      strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//确认返回值正确
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"597952366\",\"323641\",\"1234567890\"\r\n");//去登录MQTT服务器，设备ID，产品ID,鉴权.用户根据实际需要进行更改
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}



//访问ONENET需要提交JSON数据
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
