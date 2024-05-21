#include "stm32f10x.h"
#include "Serial.h"
extern uint8_t RxLen;
extern char Complete_order[50];
/**
  * ��    ������ȫFPM133������   ��ӹ̶�ͷ���̶����룬У��λ
  * ��    ������һ������Ĳ���Ϊ�������ͺ��������ݣ�Len_OrderΪ��������ĳ��ȣ����ȫ����������Oarray����
  * �� �� ֵ����
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
	Oarray[9]=5+Len_Order;   //��һ��5ΪУ������4+1(У��1�ֽ�),��������2�ֽڣ�����ʵ������5�ֽ�
	
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
	Complete_Order(Array,Len_Order, Complete_order);     //��ȫ�����ӹ̶�֡ͷ�����2��У���룬Ĭ������00000000
	Uart3_SendCharArray(Complete_order,Len_Order+16);                     //����3������������
}
