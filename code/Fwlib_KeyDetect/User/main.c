#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

int main()
{
	uint8_t key = 0;
	LED_GPIO_Config();
	Key_GPIO_Config();
	
	while(1)
	{
		//key = Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN); //���K1����
		key = Key_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN); //���K2����
		if(key == KEY_ON)
			//ÿ����KEY1����һ�Σ�LED_G��״̬��ת��һ��
			LED_G_TOGGLE;
	}
	return 0;
}

