#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_gpio_exti.h"

//����KEY1��KEY2����EXTI�жϣ�KEY1-->EXTI0   KEY2-->EXTI13

int main()
{
	LED_GPIO_Config();
	KEY_EXTI_Config();
	
	
	while(1)
	{
		
	}
	
	return 0;
}

