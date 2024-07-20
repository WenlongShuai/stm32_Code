#include "bsp_advancedTim.h"

//由于基本定时器TIM6、TIM7没有外部GPIO，所以不需要初始化GPIO

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
	
	//打开基本定时器的时钟
	ADVANCEDTIM_CLK_FUNC(ADVANCEDTIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //时钟分频因子，此成员用于通用和高级定时器的外部输入时钟时的输入滤波的分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ADVANCEDTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCEDTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //重复计数，只用于高级定时器TIM1、TIM8
	
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

