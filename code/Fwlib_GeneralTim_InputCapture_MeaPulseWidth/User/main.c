#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_generalTim_InputCapture_MeaPulseWidth.h"


/* ʹ��ͨ�ö�ʱ��TIM2~TIM5��ȡ�����ȣ�Ϊ��ʵ�鷽�㣬�˴�ʹ�õ���TIM5��Channel1��PA0��ֱ�Ӽ��KEY1�������µ�ʱ�䣨�����ȣ� */


void delay(uint32_t time);

int main()
{
	uint32_t time = 0;
	uint16_t singleCountTime = (GENERALTIM_PRESCALER + 1) / 72;  //us����ʱ������������һ�ε�ʱ��
	
	LED_GPIO_Config();
	USART_GPIO_Init();
	GeneralTim_Init();
	
	LED_G(ON);
	
	printf ( "\r\nSTM32 ���벶��ʵ��\r\n" );
	printf ( "\r\n����K1������K1���µ�ʱ��\r\n" );
	
	while(1)
	{
		if(TIM_ICUserValueStructure.Capture_FinishFlag == 1)
		{
			time = (TIM_ICUserValueStructure.Capture_Period*(GENERALTIM_PERIOD+1)+
						 TIM_ICUserValueStructure.Capture_CcrValue+1)*singleCountTime;
			
			// ��ӡ�ߵ�ƽ����ʱ��
			printf ( "\r\n��øߵ�ƽ����ʱ�䣺%f s\r\n",time/1000000.0);
			
			TIM_ICUserValueStructure.Capture_FinishFlag = 0;
		}
	}
	
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}




