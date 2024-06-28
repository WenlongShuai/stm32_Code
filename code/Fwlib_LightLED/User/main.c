#include "stm32f10x.h"
#include "bsp_led.h"

//ʵ��������ɫ��LED��ˮ��

void Delay(uint32_t time);

int main()
{
	LED_GPIO_Config();  //��ʼ��GPIOB
	
	while(1)
	{
		#if 0
		//GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);  //����LED_GPIO_PINΪ1���ر�LED
		LED_G(OFF);  //ʹ�ú궨��ķ�ʽ����LED
		Delay(0xFFFFF);
		//GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);  //���LED_GPIO_PINΪ0������LED
		LED_G(ON);
		Delay(0xFFFFF);
		
		#elif 1
		LED_R(ON);
		LED_G(OFF);
		LED_B(OFF);
		Delay(0x5FFFFF);
		
		LED_R(OFF);
		LED_G(ON);
		LED_B(OFF);
		Delay(0x5FFFFF);
		
		LED_R(OFF);
		LED_G(OFF);
		LED_B(ON);
		Delay(0x5FFFFF);
		
		#endif
	}
	
	
	return 0;
}


void Delay(uint32_t time)
{
	while(time--);
}
