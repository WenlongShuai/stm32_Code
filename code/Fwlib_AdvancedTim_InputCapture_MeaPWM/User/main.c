#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_advancedTim_InputCapture_MeaPWM.h"
#include "bsp_generalTim_Output_PWM.h"


/* ʹ�ø߼���ʱ��TIM1��TIM8ʵ������PWM�źŲ������PWM��ռ�ձȺ�Ƶ�� */

extern ICxCountValueStruct ICxCountValue;

int main()
{
	float DutyCycle = 0;
	float Frequency = 0;
	float singleCountFre = 72000000 / (ADVANCEDTIM_PRESCALER+1);
	
	USART_GPIO_Init();
	LED_GPIO_Config();
	GeneralTim_Init();
	AdvancedTim_Init();
	
	LED_B(ON);
	
	while(1)
	{
		if(ICxCountValue.IC1Value != 0)
		{
			printf("IC1Value = %d  IC2Value = %d ",ICxCountValue.IC1Value,ICxCountValue.IC2Value);
			
			Frequency = singleCountFre / (ICxCountValue.IC1Value+1);  //Hz
			
			DutyCycle = ((ICxCountValue.IC2Value+1)*100) / (ICxCountValue.IC1Value+1);
			
			printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",DutyCycle,Frequency);
			
		}
		else
		{
			Frequency = 0;
			DutyCycle = 0;
		}
	}
	
	
	return 0;
}




