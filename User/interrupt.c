#include "stm32f10x.h"
#include "GUI.h"
#include "Lcd_Driver.h"
extern int state; //指纹模块状态 0表示休眠 1表示识别 2表示录入

void EXTI9_5_IRQHandler(void) { 
	if (EXTI_GetITStatus(EXTI_Line6) == SET) {	// 判断 PX10 对应的挂起寄存器是否置位
		// 处理你的中断 ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"当前:录入");
		state=2;
		EXTI_ClearITPendingBit(EXTI_Line6);	// 清除中断标志位
	}
	if (EXTI_GetITStatus(EXTI_Line7) == SET) {	// 判断 PX10 对应的挂起寄存器是否置位
		// 处理你的中断 ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"当前:识别");
		state=1;
		EXTI_ClearITPendingBit(EXTI_Line7);	// 清除中断标志位
	}
	if (EXTI_GetITStatus(EXTI_Line8) == SET) {	// 判断 PX10 对应的挂起寄存器是否置位
		// 处理你的中断 ...
		Gui_DrawFont_GBK16(16,120,RED,GRAY0,"当前:休眠");
		state=0;
		EXTI_ClearITPendingBit(EXTI_Line8);	// 清除中断标志位
	}
}

void exti_init(void)
{
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  //打开PB口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO 时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8;  //暂时只需要这2个键

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //端口模式，此为输入上拉模式
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);         //初始化对应的端口
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);	// 把 6 号引脚的中断配给 GPIOB，即 PB6
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);	// 把 7 号引脚的中断配给 GPIOB，即 PB7
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);	// 把 7 号引脚的中断配给 GPIOB，即 PB8
	
    NVIC_InitTypeDef NVIC_InitStructure;

	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8;		// 选择中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		// 表示开放该条中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// 中断模式，事件模式为 EXTI_Mode_EVENT，唔不过这是个枚举类型，难道不能一起开吗
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// 以上升沿触发为例
	EXTI_Init(&EXTI_InitStructure);

	
	// 配置NVIC以启用EXTI线路中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // 设置NVIC优先级分组1
	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; // 设置中断通道为EXTI0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
    NVIC_Init(&NVIC_InitStructure); // 初始化NVIC
	
 
}