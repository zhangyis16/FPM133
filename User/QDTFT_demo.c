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
				 while(!S1);//�ȴ������ͷ�

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
		if(!S3)//K3:#�༭
		{
			delay_ms(10);
			if(!S3)	
			{
				 while(!S3);
				
				edit=1;//����༭״̬
			}
		}
	///////////////////////////////////////////
		if(!S4)//K4:*ȷ�ϣ�����
		{
			delay_ms(10);
			if(!S4)	
			{
				 while(!S4);
				edit=0;//�˳��༭״̬�����ز˵�
				first_in=0;
				}
			}
		if(first_in==0)
	{
		Lcd_Clear(GRAY0);
		//Gui_DrawFont_GBK16(16,0,BLUE,GRAY0,"ȫ�����Ӽ���");
	  Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Һ�����Գ���");
		Gui_DrawFont_GBK16(16,40,GREEN,GRAY0,"��ɫ������");
		Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");
		Gui_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");
		Gui_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");
		first_in=1;
	}
	if(edit==0)
	{
		if(key_count==1)//ѡ��
		{
			DisplayButtonDown(15,38,113,58); //x1,y1,x2,y2
		}
		else
		{
			DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
		}
		

		
		if(key_count==2)//ѡ��
		{
			DisplayButtonDown(15,68,113,88); //x1,y1,x2,y2
		}
		else
		{
			DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
		}
		
		if(key_count==3)//ѡ��
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
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"������ʾ����");

	delay_ms(1000);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,30,YELLOW,GRAY0,"���Ӽ���");
	Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"רעҺ������");
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "ȫ�̼���֧��");
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

//ȡģ��ʽ ˮƽɨ�� ������ ��λ��ǰ
void showimage(const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_Clear(WHITE); //����  
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j+2,40*k,40*j+39,40*k+39);		//��������
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}
void QDTFT_Test_Demo(void)
{
	
	LCD_LED_SET;//ͨ��IO���Ʊ�����				
	Redraw_Mainmenu();//�������˵�(�����������ڷֱ��ʳ�������ֵ�����޷���ʾ)
	if(key_count==1&&edit==1)
	{
		Color_Test();//�򵥴�ɫ������
		edit=0;//��ʾһ�ξ��˳����������˵�
		first_in=0;//��ʾһ�ξ��˳������½������˵�
	}
	else if(key_count==2&&edit==1)
	{
		Num_Test();//������������
		Font_Test();//��Ӣ����ʾ����	
		edit=0;//��ʾһ�ξ��˳����������˵�
		first_in=0;//��ʾһ�ξ��˳������½������˵�	
	}
	else if(key_count==3&&edit==1)
	{
		showimage(gImage_qq);//ͼƬ��ʾʾ��
		delay_ms(1000);
		edit=0;//��ʾһ�ξ��˳����������˵�
		first_in=0;//��ʾһ�ξ��˳������½������˵�
	}
	
	//delay_ms(1200);
	//LCD_LED_CLR;//IO���Ʊ�����	
	
}
