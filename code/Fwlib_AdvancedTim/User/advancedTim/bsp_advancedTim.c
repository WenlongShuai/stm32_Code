#include "bsp_advancedTim.h"

//���ڻ�����ʱ��TIM6��TIM7û���ⲿGPIO�����Բ���Ҫ��ʼ��GPIO

uint16_t time = 0;

static void AdvancedTim_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = ADVANCEDTIM_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_Init(&NVIC_InitStruct);
}

static void AdvancedTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//�򿪻�����ʱ����ʱ��
	ADVANCEDTIM_CLK_FUNC(ADVANCEDTIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ��˳�Ա����ͨ�ú͸߼���ʱ�����ⲿ����ʱ��ʱ�������˲��ķ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ADVANCEDTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCEDTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //�ظ�������ֻ���ڸ߼���ʱ��TIM1��TIM8
	
	TIM_TimeBaseInit(ADVANCEDTIMx, &TIM_TimeBaseInitStruct);
	
	TIM_ClearITPendingBit(ADVANCEDTIMx, TIM_IT_Update);
	
	TIM_ITConfig(ADVANCEDTIMx, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(ADVANCEDTIMx, ENABLE);
}

void AdvancedTim_Init(void)
{
	AdvancedTim_NVIC_Config();
	AdvancedTim_Config();
}

void AdvancedTim_Delay_ms(uint16_t ms)
{
	while(1)
	{
		if(time == ms)
		{
			time = 0;
			break;
		}
	}
}

