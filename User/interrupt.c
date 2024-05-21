#include "stm32f10x.h"
#include "GUI.h"
#include "Lcd_Driver.h"
extern int state; //ָ��ģ��״̬ 0��ʾ���� 1��ʾʶ�� 2��ʾ¼��

void EXTI9_5_IRQHandler(void) { 
	if (EXTI_GetITStatus(EXTI_Line6) == SET) {	// �ж� PX10 ��Ӧ�Ĺ���Ĵ����Ƿ���λ
		// ��������ж� ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"��ǰ:¼��");
		state=2;
		EXTI_ClearITPendingBit(EXTI_Line6);	// ����жϱ�־λ
	}
	if (EXTI_GetITStatus(EXTI_Line7) == SET) {	// �ж� PX10 ��Ӧ�Ĺ���Ĵ����Ƿ���λ
		// ��������ж� ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"��ǰ:ʶ��");
		state=1;
		EXTI_ClearITPendingBit(EXTI_Line7);	// ����жϱ�־λ
	}
	if (EXTI_GetITStatus(EXTI_Line8) == SET) {	// �ж� PX10 ��Ӧ�Ĺ���Ĵ����Ƿ���λ
		// ��������ж� ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"��ǰ:����");
		state=0;
		EXTI_ClearITPendingBit(EXTI_Line8);	// ����жϱ�־λ
	}
}

void exti_init(void)
{
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  //��PB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8;  //��ʱֻ��Ҫ��2����

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //�˿�ģʽ����Ϊ��������ģʽ
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);         //��ʼ����Ӧ�Ķ˿�
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);	// �� 6 �����ŵ��ж���� GPIOB���� PB6
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);	// �� 7 �����ŵ��ж���� GPIOB���� PB7
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);	// �� 7 �����ŵ��ж���� GPIOB���� PB8
	
    NVIC_InitTypeDef NVIC_InitStructure;

	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8;		// ѡ���ж���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		// ��ʾ���Ÿ����ж���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// �ж�ģʽ���¼�ģʽΪ EXTI_Mode_EVENT���������Ǹ�ö�����ͣ��ѵ�����һ����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// �������ش���Ϊ��
	EXTI_Init(&EXTI_InitStructure);

	
	// ����NVIC������EXTI��·�ж�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // ����NVIC���ȼ�����1
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; // �����ж�ͨ��ΪEXTI0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC
	
 
}