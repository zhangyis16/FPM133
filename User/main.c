//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//  ��������   : 1.8��LCD 4�ӿ���ʾ����(STM32ϵ��)
//               4������ѡ����Ե�����
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4 
//							BL		��PB5
//							K1    ��PB6
//							K2    ��PB7
//							K3    ��PB8
//							K4    ��PB9
*******************************************************************************/



#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "QDTFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "key.h"
#include "misc.h"
#include "interrupt.h"
#include "Serial.h"
#include "FPM133.h"
#include "stdio.h"

int state=1; //ָ��ģ��״̬ 0��ʾ���� 1��ʾʶ�� 2��ʾ¼��

char Complete_order[50]={0};

char Query_Temp_Num[3] ={0x02,0x03,0};  //��ѯָ��ģ����������Ҫ��fpm133�е�  Complete_Order��ȫ
char System_Strate[2] ={0x02,0xfb};  //��ȡϵͳ����

char Get_ID[2] ={0x03,0x01};  //��ȡϵͳ����


char Turn_On_Light_Order[7] ={0x02,0x0f,0x01,0x01,0x14,0x14,0x05};  //����ָ��ģ��FP133  LED������ɫ
char Turn_Off_Light_Order[7] ={0x02,0x0f,0x00,0x01,0x14,0x14,0x05};  //����ָ��ģ��FP133  LED�ر�

char Finger_Reg[3] ={0x01,0x11,0x00};  //ָ��ע��ָ���������Ϊע���� 1~6
char Query_Reg[2] ={0x01,0x12};  //ѯ��ָ��ע����ָ��
char Save_Temp[4] ={0x01,0x13};  //����ָ��ģ������
char Query_Save_Temp_Result[2] ={0x01,0x13};  //��ѯָ�Ʊ�����

char Update_Stored_Feature_Order[4] ={0x01,0x16,0x00,0x00};  //���´洢����ֵ�����Ҫ��fpm133�е�  Complete_Order��ȫ
char Update_Feature_Result[2] ={0x01,0x17};  //���´洢����ֵ�����Ҫ��fpm133�е�  Complete_Order��ȫ


//char Turn_Flash_LightPWM[23]={0xF1,0x1F,0xE2,0x2E,0xB6,0x6B,0xA8,0x8A,0x00,0x0C,0x81,0x00,0x00,0x00,0x00,0x02,0x0f,0x03,0x03,0x64,0x00,0x32,0x53}; //����ָ��ģ��FP133  LED��˸ģʽ

//��ѯ��ָ��λ״̬
char    Query_Finger_State[2]={0x01,0x35};

//ָ��ƥ��
char    Finger_Match[2]={0x01,0x21};

//��ѯƥ����                                     
char    Query_Finger_Match_Result[2]={0x01,0x22};


extern char Serial_RxPacket[100];				//����������ݰ����飬���ݰ���ʽ"@MSG\r\n"
extern uint8_t RxLen;

int main(void)
{
	
	
	SystemInit();	//System init.
    delay_init(72);//Delay init.
    //KEY_Init();//������ʼ��
    Lcd_Init();
	
	LCD_LED_SET;//ͨ��IO���Ʊ�����	
    Lcd_Clear(WHITE); 
    Gui_DrawFont_GBK16(16,20,RED,GRAY0,"TestOpendoor");
    Gui_DrawFont_GBK16(16,40,RED,GRAY0,"¼��ָ����:");
	
	Gui_DrawFont_GBK16(16,80,RED,GRAY0,"k1¼��k2ʶ��");
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"k3����");
	Gui_DrawFont_GBK16(16,120,RED,GRAY0,"��ǰ:ʶ��");
	delay_ms(150);

	uart3_init();	
	uart1_Init();//����1��ʼ��
	
		
	
	Send_Order(Turn_On_Light_Order, 7);              //��FPM133ָ��ģ��LED��
	delay_ms(500);
	//Uart1_SendCharArray(Serial_RxPacket,23);   //����1�����Իش�ָ��ģ�鷵�ص���Ϣ
	 
	Send_Order(Turn_Off_Light_Order, 7);                   //�ر�FPM133ָ��ģ��LED��
	delay_ms(200);
	//Uart1_SendCharArray(Serial_RxPacket,23);

	


	Send_Order(Query_Temp_Num,3);
	delay_ms(200);
	//Uart1_SendCharArray(Serial_RxPacket,23);
	int num=Serial_RxPacket[22];
	u8 ch[2]={num/10+48,num%10+48};
	Gui_DrawFont_GBK16(16,60,RED,GRAY0,ch);

    exti_init();
	
	while(1)
	{
		if (state==1)
		{
			Send_Order(Finger_Match,2);
			delay_ms(150);
			//Uart1_SendCharArray(Serial_RxPacket,23);
			while (Serial_RxPacket[20]!=0)          //�ȴ�����ƥ��ָ��
			{
				Send_Order(Finger_Match,2);
				delay_ms(150);
			}
			
			Send_Order(Query_Finger_Match_Result,2);
			delay_ms(100);
			while (Serial_RxPacket[20]==4)    //ƥ��ʧ�ܣ�ϵͳ��æ
			{
				Send_Order(Query_Finger_Match_Result,2);
				delay_ms(100);
			}
			if (Serial_RxPacket[20]==0)
			{
				int pipeijieguo=Serial_RxPacket[21]*256+Serial_RxPacket[22];
				int fenshu=Serial_RxPacket[23]*256+Serial_RxPacket[24];
				int id=Serial_RxPacket[25]*256+Serial_RxPacket[26];
			
			
			//Serial_SendNumber(fenshu,4);
			//Serial_SendNumber(id,4);			//����1����ƥ����
				if (pipeijieguo==1)
				{
					//Gui_DrawFont_Num32(16,70,RED,GRAY0,id%10);
					Send_Order(Turn_On_Light_Order, 7);              //��FPM133ָ��ģ��LED��
					delay_ms(100);
				
					Send_Order(Update_Stored_Feature_Order, 4);     //���´洢����ֵ���� 
					delay_ms(300);
				
					Send_Order(Update_Feature_Result, 2);                         //��ѯ����ֵ���½��
					delay_ms(100);
					
					Send_Order(Query_Finger_State,2);//��ѯ��ָ��λ״̬
					delay_ms(100);

					while (Serial_RxPacket[21]==1)          //��ָ��λ
					{
						Send_Order(Query_Finger_State,2);//��ѯ��ָ��λ״̬
						delay_ms(100);
					}
					Send_Order(Turn_Off_Light_Order, 7);                   //�ر�FPM133ָ��ģ��LED��
					delay_ms(100);
				}
			}
		}	
		else if (state==2)
		{
			int id_h=0,id_l=1;
			for(int i=1;i<=6;i++)
			{
				Finger_Reg[2]=i;
				Send_Order(Finger_Reg, 3);  //ѯ���Ƿ����ָ��ע��
				delay_ms(100);
				
				while (Serial_RxPacket[20]!=0)
				{
					Send_Order(Finger_Reg, 3);  //ѯ���Ƿ����ָ��ע��
					delay_ms(100);
				}
				//Uart1_SendCharArray(Serial_RxPacket,22);
				
				Send_Order(Query_Reg, 2);	//��ѯָ��ע����
				delay_ms(200);
				
				while (Serial_RxPacket[20]==4)  //ϵͳæ ��ȴ�
				{
					Send_Order(Query_Reg, 2);	//��ѯָ��ע����
					delay_ms(200);
				}
				
				Uart1_SendCharArray(Serial_RxPacket,25);
				
				if (Serial_RxPacket[20]==0)  //ע��ɹ�
				{
					Gui_DrawFont_GBK16(16,140,RED,GRAY0,"�� �γɹ�");
					u8 ch1[]={i+48};
					Gui_DrawFont_GBK16(32,140,RED,GRAY0,ch1);
					if (i==6)
					{
						id_h=Serial_RxPacket[21],id_l=Serial_RxPacket[22];
					}
					Send_Order(Query_Finger_State,2);//��ѯ��ָ��λ״̬
					delay_ms(200);
					while (Serial_RxPacket[21]==1)          //��ָ��λ
					{
						Send_Order(Query_Finger_State,2);//��ѯ��ָ��λ״̬
						delay_ms(200);
					}
				}else
				{
					i--;
				}
				if (Serial_RxPacket[20]==15)  //ָ��ģ���ظ�
				{
					Gui_DrawFont_GBK16(16,140,RED,GRAY0,"ָ���ظ�");
				}
			}
			
			Save_Temp[2]=id_h; Save_Temp[3]=id_l;
			Send_Order(Save_Temp,4);  //����ָ��ģ��
			delay_ms(200);
			Uart1_SendCharArray(Serial_RxPacket,22);
			
			Send_Order(Query_Save_Temp_Result,2);//��ѯָ�Ʊ�����
			delay_ms(200);
			Uart1_SendCharArray(Serial_RxPacket,24);
			
	
			Send_Order(Query_Temp_Num,3);
			delay_ms(200);
			Uart1_SendCharArray(Serial_RxPacket,24);
			int num=Serial_RxPacket[22];
			u8 ch[2]={num/10+48,num%10+48};
			Gui_DrawFont_GBK16(16,60,RED,GRAY0,ch);
			
			Gui_DrawFont_GBK16(16,140,WHITE,GRAY0,"         ");
	
			state=0;
			Gui_DrawFont_GBK16(16,120,RED,GRAY0,"��ǰ:����");
		}
	}
}


