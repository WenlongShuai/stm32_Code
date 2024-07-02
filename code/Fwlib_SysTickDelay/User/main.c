#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"

//SysTick�ֱ�ʹ�üĴ����������̼�������Լ��жϲ���������ʱ

int main()
{
	LED_GPIO_Config();
	
	while(1)
	{
		#if SYSTICK_REG
		LED_G(ON);
		SysTick_REG_Delay_us(500000);
		//SysTick_REG_Delay_ms(50);
		LED_G(OFF);
		SysTick_REG_Delay_us(500000);
		//SysTick_REG_Delay_ms(50);
		
		#elif SYSTICK_FWLIB
		LED_G(ON);
		//SysTick_FWLIB_Delay_us(50000);
		SysTick_FWLIB_Delay_ms(1000);
		LED_G(OFF);
		//SysTick_FWLIB_Delay_us(50000);
		SysTick_FWLIB_Delay_ms(1000);
		
		#elif SYSTICK_INT
		LED_G(ON);
		SysTick_INT_Delay_us(50000);
		//SysTick_INT_Delay_ms(50);
		LED_G(OFF);
		SysTick_INT_Delay_us(50000);
		//SysTick_INT_Delay_ms(50);
		
		#endif
	}
		
		
	return 0;
}

