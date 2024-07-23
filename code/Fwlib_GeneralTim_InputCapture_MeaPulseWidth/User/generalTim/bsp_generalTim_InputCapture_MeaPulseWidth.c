#include "bsp_generalTim_InputCapture_MeaPulseWidth.h"

// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0};

//����TIM���ж����ȼ�
static void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = GENERALTIM_IRQ;
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
	GENERALTIM_GPIO_CLK_FUNC(GENERALTIM_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GENERALTIM_CHANNEL1_PIN;
	GPIO_Init(GENERALTIM_CHANNEL1_PORT, &GPIO_InitStruct);
}

static void GeneralTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//�򿪻�����ʱ����ʱ��
	GENERALTIM_CLK_FUNC(GENERALTIM_CLK, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ��˳�Ա����ͨ�ú͸߼���ʱ�����ⲿ����ʱ��ʱ�������˲��ķ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = GENERALTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = GENERALTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //�ظ�������ֻ���ڸ߼���ʱ��TIM1��TIM8	
	TIM_TimeBaseInit(GENERALTIMx, &TIM_TimeBaseInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = GENERALTIM_CHANNELx;
	TIM_ICInitStruct.TIM_ICFilter = 0x00;  //���˲�������fDTS����
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;  //�����ؼ��
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //��Ԥ��Ƶ��������������ϼ�⵽��ÿһ�����ض�����һ�β���
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ͨ��ʹ��ֱ�����ӷ�ʽ
	
	TIM_ICInit(GENERALTIMx, &TIM_ICInitStruct);
	
	TIM_ClearITPendingBit(GENERALTIMx, GENERALTIM_CHANNEL_IT_CCx | TIM_IT_Update); //���CC1���жϱ�־λ�͸����жϱ�־
	
	TIM_ITConfig(GENERALTIMx, GENERALTIM_CHANNEL_IT_CCx | TIM_IT_Update, ENABLE);  //ʹ���ж�
	
	TIM_Cmd(GENERALTIMx, ENABLE);	
}

void GeneralTim_Init(void)
{
	NVIC_Config();
	GPIO_Config();
	GeneralTim_Config();
}
