#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_pvd.h"

/* 
stm32��Դ����Ԫ�еĿɱ�̵�ѹ�����PVD������ʵʱ���VDD�ĵ�ѹ��
����⵽����/�����������õĵ�ѹʱ�ͻ����һ��PVD�ж�(EXTI16�ж�) 
*/

int main()
{
	LED_GPIO_Config();
	USART_GPIO_Init();
	pvd_Config();
	
	LED_G(ON);//����������̵�
	
	while(1)
	{
	
	}
	
	return 0;
}

