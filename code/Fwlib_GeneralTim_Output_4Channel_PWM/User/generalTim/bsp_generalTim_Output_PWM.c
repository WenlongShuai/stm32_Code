#include "bsp_generalTim_Output_PWM.h"

static void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��GPIO�����ʱ��
	GENERALTIM_GPIO_CLK_FUNC(GENERALTIM_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = GENERALTIM_CHANNEL1_PIN;
	GPIO_Init(GENERALTIM_CHANNEL1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GENERALTIM_CHANNEL2_PIN;
	GPIO_Init(GENERALTIM_CHANNEL2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GENERALTIM_CHANNEL3_PIN;
	GPIO_Init(GENERALTIM_CHANNEL3_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GENERALTIM_CHANNEL4_PIN;
	GPIO_Init(GENERALTIM_CHANNEL4_PORT, &GPIO_InitStruct);
}

static void GeneralTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//�򿪻�����ʱ����ʱ��
	GENERALTIM_CLK_FUNC(GENERALTIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ��˳�Ա����ͨ�ú͸߼���ʱ�����ⲿ����ʱ��ʱ�������˲��ķ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = GENERALTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = GENERALTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //�ظ�������ֻ���ڸ߼���ʱ��TIM1��TIM8
	
	TIM_TimeBaseInit(GENERALTIMx, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;  //�˳�Աֻ������TIM1��TIM8��ͨ�ö�ʱ�����ù�
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  //�˳�Աֻ������TIM1��TIM8��ͨ�ö�ʱ�����ù�
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;  //�˳�Աֻ������TIM1��TIM8��ͨ�ö�ʱ�����ù�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;  //�˳�Աֻ������TIM1��TIM8��ͨ�ö�ʱ�����ù�
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OCInitStruct.TIM_Pulse = GENERALTIM_CHANNEL1_PULSE;  //Channel1��ռ�ձ�
	TIM_OC1Init(GENERALTIMx, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(GENERALTIMx, TIM_OCPreload_Enable);
	
	TIM_OCInitStruct.TIM_Pulse = GENERALTIM_CHANNEL2_PULSE;  //Channel2��ռ�ձ�
	TIM_OC2Init(GENERALTIMx, &TIM_OCInitStruct);
	TIM_OC2PreloadConfig(GENERALTIMx, TIM_OCPreload_Enable);
	
	TIM_OCInitStruct.TIM_Pulse = GENERALTIM_CHANNEL3_PULSE;  //Channel3��ռ�ձ�
	TIM_OC3Init(GENERALTIMx, &TIM_OCInitStruct);
	TIM_OC3PreloadConfig(GENERALTIMx, TIM_OCPreload_Enable);

	TIM_OCInitStruct.TIM_Pulse = GENERALTIM_CHANNEL4_PULSE;  //Channel4��ռ�ձ�
	TIM_OC4Init(GENERALTIMx, &TIM_OCInitStruct);
	TIM_OC4PreloadConfig(GENERALTIMx, TIM_OCPreload_Enable);

	TIM_Cmd(GENERALTIMx, ENABLE);	
}

void GeneralTim_Init(void)
{
	GPIO_Config();
	GeneralTim_Config();
}
