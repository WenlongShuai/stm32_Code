#include "stm32f10x.h"
#include "bsp_fullcolor_led.h"
#include "bsp_usart.h"

/* 
stm32 ʹ��SRAM���Դ���
��Ҫ���裺
1����ԭ���̻����ϴ���һ�����԰汾��ֻ��Ҫ����һ��ִ���ļ��������ļ��������汾����һ�¡�
2���޸ķ�ɢ�����ļ���ʹ�������Ѵ�����䵽�ڲ�SRAM�ռ䡣
3����Ӻ��޸�STM32���������ַ��
4���޸ķ������������������ã�ʹ�����ܹ�ͨ���������洢��SRAM�ռ䡣
5������ʹ�����ѡ���Ƿ���Ҫʹ�÷���������ű��ļ�*.ini��
6�����Ը�SRAM���س�����߷�����ԡ�
*/

void delay(uint32_t time);

int main()
{
	USART_GPIO_Init();
	FullColor_LED_Init();
	
	while(1)
	{
		setRGBColor(0x805009);
		delay(0x5FFFFF);
		
		setRGBColor(0x20FFF0);
		delay(0x5FFFFF);
		
		setRGBColor(0xAAC0FF);
		delay(0x5FFFFF);
	}
	return 0;
}


void delay(uint32_t time)
{
	while(time--);
}



