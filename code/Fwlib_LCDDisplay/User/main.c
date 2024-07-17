#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_lcd_ili9341.h"

int main()
{
	uint16_t data = 0;
	
	LCD_FSMC_Init();  //��FSMC��ʼ����������USART��ʼ������֮ǰ��������ĳЩ�����mcu���ܷ�
	
	USART_GPIO_Init();
	
	//drawPoint(10, 10, RGB888_CONVERT_RGB565(0xFF, 0, 0));
	
	drawRect(60, 60, 100, 40, RGB888_CONVERT_RGB565(0, 0xFF, 0));
	drawRect(50, 150, 100, 40, RGB888_CONVERT_RGB565(0xFF, 0xFF, 0));
	while(1)
	{
		
	}
	return 0;
}

