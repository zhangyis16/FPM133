//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//  ��������   : 1.8��LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4 
//							BL		��ΪPB5
*******************************************************************************/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "LCD_Config.h"
#include "delay.h"

#define ST7735_MADCTL     0x36
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_RGB 0x00

//Һ��IO��ʼ������
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_5| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//��SPI���ߴ���һ��8λ����
void  SPI_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	  LCD_SDA_SET; //�������
      else LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}

//��Һ����дһ��8λָ��
void Lcd_WriteIndex(u8 Index)
{
   //SPI д����ʱ��ʼ
   LCD_CS_CLR;
   LCD_RS_CLR;
	 SPI_WriteData(Index);
   LCD_CS_SET;
}

//��Һ����дһ��8λ����
void Lcd_WriteData(u8 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData(Data);
   LCD_CS_SET; 
}
//��Һ����дһ��16λ����
void LCD_WriteData_16Bit(u16 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//д���8λ����
	 SPI_WriteData(Data); 			//д���8λ����
   LCD_CS_SET; 
}

void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);
}
void ST7735_SetRotation(uint8_t rotation)
{
    uint8_t madctl = 0;

    switch (rotation)
    {
        case 0:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
            break;
        case 1:
            madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
            break;
        case 2:
            madctl = ST7735_MADCTL_RGB;
            break;
        case 3:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
            break;
    }

    Lcd_WriteIndex(ST7735_MADCTL);
    Lcd_WriteData(madctl);
}
//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void)
{	
	LCD_GPIO_Init();
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	delay_ms (120);
	
	Lcd_WriteIndex(0xB1);
	Lcd_WriteData(0x00);//0x05	 
	Lcd_WriteData(0x08);//0x3A	 
	Lcd_WriteData(0x05);//0x3A
	Lcd_WriteIndex(0xB2);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x3A);
	Lcd_WriteIndex(0xB3);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x05);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x3A);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	Lcd_WriteIndex(0xB4); //Dot inversion
	Lcd_WriteData(0x03);
	//------------------------------------ST7735S Power Sequence-----------------------------------------//
	Lcd_WriteIndex(0xC0);
	Lcd_WriteData(0x62);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x04);
	Lcd_WriteIndex(0xC1);
	Lcd_WriteData(0xC0);
	Lcd_WriteIndex(0xC2);
	Lcd_WriteData(0x0D);
	Lcd_WriteData(0x00);
	Lcd_WriteIndex(0xC3);
	Lcd_WriteData(0x8D);
	Lcd_WriteData(0x6A);
	Lcd_WriteIndex(0xC4);
	Lcd_WriteData(0x8D);
	Lcd_WriteData(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	Lcd_WriteIndex(0xC5); //VCOM
	Lcd_WriteData(0x08);//0x12
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	Lcd_WriteIndex(0xE0);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x1B);
	Lcd_WriteData(0x12);
	Lcd_WriteData(0x11);
	Lcd_WriteData(0x3F);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x32);
	Lcd_WriteData(0x34);
	Lcd_WriteData(0x2F);
	Lcd_WriteData(0x2B);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x3A);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x05);
	Lcd_WriteIndex(0xE1);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x1B);
	Lcd_WriteData(0x12);
	Lcd_WriteData(0x11);
	Lcd_WriteData(0x32);
	Lcd_WriteData(0x2F);
	Lcd_WriteData(0x2A);
	Lcd_WriteData(0x2F);
	Lcd_WriteData(0x2E);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x35);
	Lcd_WriteData(0x3F);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x05);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//

	Lcd_WriteIndex(0x3A); //65k mode
	Lcd_WriteData(0x05);
	//Lcd_WriteIndex(0x36); //65k mode
	//Lcd_WriteData(0x40);
	ST7735_SetRotation(0);//����ͼ����ת�Ƕ�
	Lcd_WriteIndex(0x29); //Display on
	Lcd_WriteIndex(0x2C);
	
	Lcd_WriteIndex(0x29);//Display on	 
	
}


/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start);//Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+0);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+1);
	
	Lcd_WriteIndex(0x2c);

}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	Lcd_SetRegion(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(Data);

}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
unsigned int Lcd_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	LCD_WriteData_16Bit(Color);
    }   
}

