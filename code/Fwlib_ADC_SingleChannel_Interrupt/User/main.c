#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_led.h"

extern uint16_t adcConversionValue;

void delay(uint32_t time);

int main()
{
	float adcConversionValueLocal= 0.0f;
	
	LED_GPIO_Config();
	USART_GPIO_Init();
	ADCx_Init();
	
	printf("����һ��ADCʵ�飬ʹ��ADC�Ķ���ģʽ����ͨ�����ж϶�ȡADC����\r\n");
	LED_G(ON);
	
	while(1)
	{
		adcConversionValueLocal = adcConversionValue / 4096.0 * 3.3;
		printf("ADCx_CHANNEL_11 = 0x%04x\r\n", adcConversionValue);
		printf("ADCx_CHANNEL_11 value = %f V\r\n", adcConversionValueLocal);
		delay(0x6FFFFF);
	}
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

