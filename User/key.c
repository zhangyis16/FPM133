/*-------------------------------------------------------------------------------
�ļ����ƣ�key.c
�ļ����������ð�����ʼ������       
��    ע����
---------------------------------------------------------------------------------*/
#include "key.h"
/*-------------------------------------------------------------------------------
�������ƣ�KEY_Init
�����������������ų�ʼ�����򿪶˿�ʱ�ӣ����ö˿����ţ��˿ڹ���Ƶ�ʣ��˿�����ģʽ 
�����������
���ز�������
��    ע����
---------------------------------------------------------------------------------*/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//��PB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//PA1,PA2,PA3,PA4��������	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	//�˿��ٶ�
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//�˿�ģʽ����Ϊ��������ģʽ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//��ʼ����Ӧ�Ķ˿�
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

