#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_AdvancedTim_ComplementaryOutput.h"


/* ʹ�ø߼���ʱ��TIM1��TIM8�������PWM����������ʱ���ɲ������ */

int main()
{
	LED_GPIO_Config();
	AdvancedTim_Init();
	
	while(1)
	{
		
	}
	
	
	return 0;
}




