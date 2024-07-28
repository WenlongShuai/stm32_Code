#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_dac.h"

/*
stm32��DAC˫ͨ���������Ҳ�����ʵ�黹ûͨ����֤����Ϊû��ʾ����
���Ҳ���Ƶ��Ϊ��
fsin = 1/Tsin = 1 / (Tsystick*(TIM_Prescaler+1)*(TIM_Period+1)*N)
N:���Ҳ��������ڵĵ���
*/

void delay(uint32_t time);

int main()
{
	LED_GPIO_Config();
	DACx_Init();
	
	LED_G(ON);
	
	while(1)
	{
	}
	
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

