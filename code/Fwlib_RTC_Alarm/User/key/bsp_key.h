#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "stm32f10x.h"
#include "bsp_led.h"

//K1 ����
#define KEY1_GPIO_PIN  GPIO_Pin_0
#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_CLK	 RCC_APB2Periph_GPIOA

//K2 ����
#define KEY2_GPIO_PIN  GPIO_Pin_13
#define KEY2_GPIO_PORT GPIOC
#define KEY2_GPIO_CLK  RCC_APB2Periph_GPIOC

#define KEY_ON   1
#define KEY_OFF  0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
					 
#endif /*__BSP_KEY_H__*/

