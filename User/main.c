//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//  功能描述   : 1.8寸LCD 4接口演示例程(STM32系列)
//               4个按键选择测试的内容
/******************************************************************************
//本程序适用与STM32F103C8
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB0
//              DC    接PB1
//              CS    接PA4 
//							BL		接PB5
//							K1    接PB6
//							K2    接PB7
//							K3    接PB8
//							K4    接PB9
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

int state=1; //指纹模块状态 0表示休眠 1表示识别 2表示录入

char Complete_order[50]={0};

char Query_Temp_Num[3] ={0x02,0x03,0};  //查询指纹模板数量，需要用fpm133中的  Complete_Order补全
char System_Strate[2] ={0x02,0xfb};  //获取系统策略

char Get_ID[2] ={0x03,0x01};  //获取系统策略


char Turn_On_Light_Order[7] ={0x02,0x0f,0x01,0x01,0x14,0x14,0x05};  //设置指纹模块FP133  LED开启绿色
char Turn_Off_Light_Order[7] ={0x02,0x0f,0x00,0x01,0x14,0x14,0x05};  //设置指纹模块FP133  LED关闭

char Finger_Reg[3] ={0x01,0x11,0x00};  //指纹注册指令，第三个数为注册数 1~6
char Query_Reg[2] ={0x01,0x12};  //询问指纹注册结果指令
char Save_Temp[4] ={0x01,0x13};  //保存指纹模板命令
char Query_Save_Temp_Result[2] ={0x01,0x13};  //查询指纹保存结果

char Update_Stored_Feature_Order[4] ={0x01,0x16,0x00,0x00};  //更新存储特征值命令，需要用fpm133中的  Complete_Order补全
char Update_Feature_Result[2] ={0x01,0x17};  //更新存储特征值命令，需要用fpm133中的  Complete_Order补全


//char Turn_Flash_LightPWM[23]={0xF1,0x1F,0xE2,0x2E,0xB6,0x6B,0xA8,0x8A,0x00,0x0C,0x81,0x00,0x00,0x00,0x00,0x02,0x0f,0x03,0x03,0x64,0x00,0x32,0x53}; //设置指纹模块FP133  LED闪烁模式

//查询手指在位状态
char    Query_Finger_State[2]={0x01,0x35};

//指纹匹配
char    Finger_Match[2]={0x01,0x21};

//查询匹配结果                                     
char    Query_Finger_Match_Result[2]={0x01,0x22};


extern char Serial_RxPacket[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"
extern uint8_t RxLen;

int main(void)
{
	
	
	SystemInit();	//System init.
    delay_init(72);//Delay init.
    //KEY_Init();//按键初始化
    Lcd_Init();
	
	LCD_LED_SET;//通过IO控制背光亮	
    Lcd_Clear(WHITE); 
    Gui_DrawFont_GBK16(16,20,RED,GRAY0,"TestOpendoor");
    Gui_DrawFont_GBK16(16,40,RED,GRAY0,"录入指纹数:");
	
	Gui_DrawFont_GBK16(16,80,RED,GRAY0,"k1录入k2识别");
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"k3休眠");
	Gui_DrawFont_GBK16(16,120,RED,GRAY0,"当前:识别");
	delay_ms(150);

	uart3_init();	
	uart1_Init();//串口1初始化
	
		
	
	Send_Order(Turn_On_Light_Order, 7);              //打开FPM133指纹模块LED灯
	delay_ms(500);
	//Uart1_SendCharArray(Serial_RxPacket,23);   //串口1往电脑回传指纹模块返回的信息
	 
	Send_Order(Turn_Off_Light_Order, 7);                   //关闭FPM133指纹模块LED灯
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
			while (Serial_RxPacket[20]!=0)          //等待可以匹配指纹
			{
				Send_Order(Finger_Match,2);
				delay_ms(150);
			}
			
			Send_Order(Query_Finger_Match_Result,2);
			delay_ms(100);
			while (Serial_RxPacket[20]==4)    //匹配失败，系统繁忙
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
			//Serial_SendNumber(id,4);			//串口1发送匹配结果
				if (pipeijieguo==1)
				{
					//Gui_DrawFont_Num32(16,70,RED,GRAY0,id%10);
					Send_Order(Turn_On_Light_Order, 7);              //打开FPM133指纹模块LED灯
					delay_ms(100);
				
					Send_Order(Update_Stored_Feature_Order, 4);     //更新存储特征值命令 
					delay_ms(300);
				
					Send_Order(Update_Feature_Result, 2);                         //查询特征值更新结果
					delay_ms(100);
					
					Send_Order(Query_Finger_State,2);//查询手指在位状态
					delay_ms(100);

					while (Serial_RxPacket[21]==1)          //手指在位
					{
						Send_Order(Query_Finger_State,2);//查询手指在位状态
						delay_ms(100);
					}
					Send_Order(Turn_Off_Light_Order, 7);                   //关闭FPM133指纹模块LED灯
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
				Send_Order(Finger_Reg, 3);  //询问是否可以指纹注册
				delay_ms(100);
				
				while (Serial_RxPacket[20]!=0)
				{
					Send_Order(Finger_Reg, 3);  //询问是否可以指纹注册
					delay_ms(100);
				}
				//Uart1_SendCharArray(Serial_RxPacket,22);
				
				Send_Order(Query_Reg, 2);	//查询指纹注册结果
				delay_ms(200);
				
				while (Serial_RxPacket[20]==4)  //系统忙 请等待
				{
					Send_Order(Query_Reg, 2);	//查询指纹注册结果
					delay_ms(200);
				}
				
				Uart1_SendCharArray(Serial_RxPacket,25);
				
				if (Serial_RxPacket[20]==0)  //注册成功
				{
					Gui_DrawFont_GBK16(16,140,RED,GRAY0,"第 次成功");
					u8 ch1[]={i+48};
					Gui_DrawFont_GBK16(32,140,RED,GRAY0,ch1);
					if (i==6)
					{
						id_h=Serial_RxPacket[21],id_l=Serial_RxPacket[22];
					}
					Send_Order(Query_Finger_State,2);//查询手指在位状态
					delay_ms(200);
					while (Serial_RxPacket[21]==1)          //手指在位
					{
						Send_Order(Query_Finger_State,2);//查询手指在位状态
						delay_ms(200);
					}
				}else
				{
					i--;
				}
				if (Serial_RxPacket[20]==15)  //指纹模板重复
				{
					Gui_DrawFont_GBK16(16,140,RED,GRAY0,"指纹重复");
				}
			}
			
			Save_Temp[2]=id_h; Save_Temp[3]=id_l;
			Send_Order(Save_Temp,4);  //保存指纹模板
			delay_ms(200);
			Uart1_SendCharArray(Serial_RxPacket,22);
			
			Send_Order(Query_Save_Temp_Result,2);//查询指纹保存结果
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
			Gui_DrawFont_GBK16(16,120,RED,GRAY0,"当前:休眠");
		}
	}
}


