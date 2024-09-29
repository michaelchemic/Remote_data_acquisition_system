#include "bc260y.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC260Y BC260Y_Status;
unsigned char socketnum=0;//当前的socket号码
void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<100;i++)
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
		printf("AT+QICLOSE=0\r\n");
		delay_ms(300);
    printf("AT+CFUN=1\r\n");//开启射频
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
     strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//返回CSQ
     Clear_Buffer();
}




void BC260Y_CreateUDPSokcet(void)//创建sokcet
{

    printf("AT+QIOPEN=0,0,\"UDP\",\"47.92.146.210\",9999,1234,1\r\n");//创建连接UDP,输入IP以及服务器端口号码 ,采用直接吐出的方式
    delay_ms(300);//UDP无连接模式，不会去检查是否登录服务器成功
    Clear_Buffer();	
	  printf("AT+QICFG=\"dataformat\",0,0\r\n");//字符模式
	  delay_ms(300);//UDP无连接模式，不会去检查是否登录服务器成功
    Clear_Buffer();	
    
}
void BC260Y_Senddata(uint8_t *len,uint8_t *data)//字符串形式
{
    printf("AT+QISEND=0,%s,%s\r\n",len,data);
	  delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//检查是否发送成功
            delay_ms(100);
		}  
	  Uart1_SendStr(RxBuffer);
		RxCounter=0;
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

void BC260Y_RECData()
{

    strx=strstr((const char*)RxBuffer,(const char*)"++QIURC:");//返回+QIURC:，表明接收到UDP服务器发回的数据
    if(strx)
    {
       

      
            Clear_Buffer();
                 
    }
}

