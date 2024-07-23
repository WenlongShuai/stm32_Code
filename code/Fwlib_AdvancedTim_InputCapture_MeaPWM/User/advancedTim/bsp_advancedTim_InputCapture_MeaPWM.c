#include "bsp_advancedTim_InputCapture_MeaPWM.h"
ICxCountValueStruct ICxCountValue = {0};
static void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = ADVANCEDTIM_CCx_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Init(&NVIC_InitStruct);
}

static void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��GPIO�����ʱ��
	ADVANCEDTIM_GPIO_CLK_FUNC(ADVANCEDTIM_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = ADVANCEDTIM_CHANNEL1_PIN;
	GPIO_Init(ADVANCEDTIM_CHANNEL1_PORT, &GPIO_InitStruct);
}

static void AdvancedTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//�򿪻�����ʱ����ʱ��
	ADVANCEDTIM_CLK_FUNC(ADVANCEDTIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ��˳�Ա����ͨ�ú͸߼���ʱ�����ⲿ����ʱ��ʱ�������˲��ķ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ADVANCEDTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCEDTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //�ظ�������ֻ���ڸ߼���ʱ��TIM1��TIM8
	TIM_TimeBaseInit(ADVANCEDTIMx, &TIM_TimeBaseInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = ADVANCEDTIM_CHANNELx;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	//TIM_ICInit(ADVANCEDTIMx, &TIM_ICInitStruct);
	
	//����TIM_ICInitStruct��ָ���Ĳ�������TIM���裬�Բ����ⲿPWM�źš�ͬʱ����CC1��CC2
	TIM_PWMIConfig(ADVANCEDTIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(ADVANCEDTIMx, TIM_TS_TI1FP1);  //ѡ����Ч�Ĵ��������ź�
	TIM_SelectSlaveMode(ADVANCEDTIMx, TIM_SlaveMode_Reset);  //���ô�ģʽ������Ϊ��λģʽ
	TIM_SelectMasterSlaveMode(ADVANCEDTIMx, TIM_MasterSlaveMode_Enable);

	
//	TIM_CCxCmd(ADVANCEDTIMx, ADVANCEDTIM_CHANNELx, TIM_CCx_Enable);  //Channel1��CC1
//	TIM_CCxCmd(ADVANCEDTIMx, TIM_Channel_2, TIM_CCx_Enable);  //Channel2��CC2
	
	
	TIM_ClearITPendingBit(ADVANCEDTIMx, TIM_IT_CC1);
	
	TIM_ITConfig(ADVANCEDTIMx, TIM_IT_CC1, ENABLE);
	
	TIM_Cmd(ADVANCEDTIMx, ENABLE);	
}

void AdvancedTim_Init(void)
{
	NVIC_Config();
	GPIO_Config();
	AdvancedTim_Config();
}
