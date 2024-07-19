#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_led.h"

extern uint32_t adcConversionValue[ADCx_1_2_CHANNEL_NUM];

void delay(uint32_t time);

int main()
{
	float adcConversionValueLocal[ADCx_1_2_CHANNEL_NUM*2]= {0.0f};
	
	USART_GPIO_Init();
	LED_GPIO_Config();
	ADCx_Init();
	
	printf("����һ��ADCʵ�飬ʹ��ADC˫��ģʽ������ͬ������ͨ����DMA��ȡADC����\r\n");
	LED_G(ON);
	
	while(1)
	{
		adcConversionValueLocal[0] = (adcConversionValue[0] & 0xFFFF) / 4096.0 * 3.3;  //ADC1
		adcConversionValueLocal[1] = ((adcConversionValue[0] & 0xFFFF0000)>>16) / 4096.0 * 3.3;  //ADC2
		
		printf("\r\n");
		printf("ADCx_1_CHANNEL_11 value = %f V\r\n", adcConversionValueLocal[0]);
		printf("ADCx_2_CHANNEL_12 value = %f V\r\n", adcConversionValueLocal[1]);
		
		delay(0x6FFFFF);
	}
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

