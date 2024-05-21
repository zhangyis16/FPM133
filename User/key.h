
#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define S1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define S2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define S3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define S4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

void KEY_Init(void);

#endif

