#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_generalTim_Output_PWM.h"


/* ʹ��ͨ�ö�ʱ��TIM2~TIM5���4·��ͬռ�ձȵ�PWM */

int main()
{
	LED_GPIO_Config();
	GeneralTim_Init();
	
	while(1)
	{
		LED_R_TOGGLE;
	}
	
	
	return 0;
}




