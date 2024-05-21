#include "stm32f10x.h"
#include "Serial.h"
extern uint8_t RxLen;
extern char Complete_order[50];
/**
  * 函    数：补全FPM133的命令   添加固定头，固定密码，校验位
  * 参    数：第一个输入的参数为命令类型和命令内容，Len_Order为传入命令的长度，最后补全的命令存放在Oarray里面
  * 返 回 值：无
  */
void Complete_Order(char *Array, unsigned int Len_Order, char *Oarray)
{
	int i;
	int sum=0;
	Oarray[0]=0xF1;
	Oarray[1]=0x1F;
	Oarray[2]=0xE2;
	Oarray[3]=0x2E;
	Oarray[4]=0xB6;
	Oarray[5]=0x6B;
	Oarray[6]=0xA8;
	Oarray[7]=0x8A;
	
	Oarray[8]=0;
	Oarray[9]=5+Len_Order;   //第一个5为校验密码4+1(校验1字节),命令类型2字节，命令实际内容5字节
	
	for(i = 0; i<10; i++) sum += Oarray[i];
	sum=(char)((~sum)+1);
	Oarray[10]=sum;
	Oarray[11]=0;
	Oarray[12]=0;
	Oarray[13]=0;
	Oarray[14]=0;
	for(i=0;i<Len_Order;i++) Oarray[i+15]=Array[i];
	sum=0;
	for(i = 11;i<11+5+Len_Order-1;i++) sum += Oarray[i];
	sum=(char)((~sum)+1);
	Oarray[15+Len_Order]=sum;
}

void Send_Order(char *Array, unsigned int Len_Order)
{
	Complete_Order(Array,Len_Order, Complete_order);     //补全命令，添加固定帧头，添加2个校验码，默认密码00000000
	Uart3_SendCharArray(Complete_order,Len_Order+16);                     //串口3发送完整命令
}
