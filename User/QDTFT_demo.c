/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"
#include "Picture.h"
#include "QDTFT_demo.h"
#include "key.h"
unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
unsigned char key_count=1;
unsigned char edit=0;
unsigned char first_in=0;

void Redraw_Mainmenu(void)
{

	//
	
	if(!S1)
		{
			delay_ms(10);
			if(!S1)	//K1
			{
				 while(!S1);//等待按键释放

				if(edit==0)
				{
					key_count=key_count-1;
					
					if(key_count<1)
					{
						key_count=3;
					}
			  }
				
			}
		}
		
		////////////////////////////////////////////
		if(!S2)//K2
		{
			delay_ms(10);
			if(!S2)	
			{
				 while(!S2);
				if(edit==0)
				{
					 key_count=key_count+1;
					 if(key_count>3)
					 {
						 key_count=1;
					 }
			  }
				
			}
		}
		//////////////////////////////////////////
		if(!S3)//K3:#编辑
		{
			delay_ms(10);
			if(!S3)	
			{
				 while(!S3);
				
				edit=1;//进入编辑状态
			}
		}
	///////////////////////////////////////////
		if(!S4)//K4:*确认，返回
		{
			delay_ms(10);
			if(!S4)	
			{
				 while(!S4);
				edit=0;//退出编辑状态，返回菜单
				first_in=0;
				}
			}
		if(first_in==0)
	{
		Lcd_Clear(GRAY0);
		//Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"全动电子技术");
	  Gui_DrawFont_GBK16(16,20,RED,GRAY0,"液晶测试程序");
		Gui_DrawFont_GBK16(16,40,GREEN,GRAY0,"颜色填充测试");
		Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"文字显示测试");
		Gui_DrawFont_GBK16(16,100,RED,GRAY0,"图片显示测试");
		Gui_DrawFont_GBK16(16,100,RED,GRAY0,"图片显示测试");
		first_in=1;
	}
	if(edit==0)
	{
		if(key_count==1)//选中
		{
			DisplayButtonDown(15,38,113,58); //x1,y1,x2,y2
		}
		else
		{
			DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
		}
		

		
		if(key_count==2)//选中
		{
			DisplayButtonDown(15,68,113,88); //x1,y1,x2,y2
		}
		else
		{
			DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
		}
		
		if(key_count==3)//选中
		{
			DisplayButtonDown(15,98,113,118); //x1,y1,x2,y2
		}
		else
		{
			DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
		}
	}
	
	
	//delay_ms(1500);
}

void Num_Test(void)
{
	u8 i=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+5,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	
}

void Font_Test(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"文字显示测试");

	delay_ms(1000);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,30,YELLOW,GRAY0,"电子技术");
	Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"专注液晶批发");
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "全程技术支持");
	Gui_DrawFont_GBK16(0,100,BLUE,GRAY0,"Tel:11111111111");
	Gui_DrawFont_GBK16(0,130,RED,GRAY0, "QQ:11111111");	
	delay_ms(1000);	
}

void Color_Test(void)
{
	u8 i=1;
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(200);

	while(i--)
	{
		Lcd_Clear(WHITE);
		Lcd_Clear(BLACK);
		Lcd_Clear(RED);
	  	Lcd_Clear(GREEN);
	  	Lcd_Clear(BLUE);
	}		
}

//取模方式 水平扫描 从左到右 低位在前
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_Clear(WHITE); //清屏  
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j+2,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}
void QDTFT_Test_Demo(void)
{
	
	LCD_LED_SET;//通过IO控制背光亮				
	Redraw_Mainmenu();//绘制主菜单(部分内容由于分辨率超出物理值可能无法显示)
	if(key_count==1&&edit==1)
	{
		Color_Test();//简单纯色填充测试
		edit=0;//演示一次就退出，进入主菜单
		first_in=0;//演示一次就退出，重新进入主菜单
	}
	else if(key_count==2&&edit==1)
	{
		Num_Test();//数码管字体测试
		Font_Test();//中英文显示测试	
		edit=0;//演示一次就退出，进入主菜单
		first_in=0;//演示一次就退出，重新进入主菜单	
	}
	else if(key_count==3&&edit==1)
	{
		showimage(gImage_qq);//图片显示示例
		delay_ms(1000);
		edit=0;//演示一次就退出，进入主菜单
		first_in=0;//演示一次就退出，重新进入主菜单
	}
	
	//delay_ms(1200);
	//LCD_LED_CLR;//IO控制背光灭	
	
}
