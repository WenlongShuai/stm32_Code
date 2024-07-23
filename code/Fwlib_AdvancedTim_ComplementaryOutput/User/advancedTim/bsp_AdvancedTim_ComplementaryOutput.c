#include "bsp_AdvancedTim_ComplementaryOutput.h"

static void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��GPIO�����ʱ��
	ADVANCEDTIM_GPIO_CLK_FUNC(ADVANCEDTIM_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = ADVANCEDTIM_CHANNEL1_PIN;
	GPIO_Init(ADVANCEDTIM_CHANNEL1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADVANCEDTIM_CHANNEL1_N_PIN;
	GPIO_Init(ADVANCEDTIM_CHANNEL1_N_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADVANCEDTIM_BKIN_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ADVANCEDTIM_BKIN_PORT, &GPIO_InitStruct);
	
	// BKIN����Ĭ��������͵�ƽ
	GPIO_ResetBits(ADVANCEDTIM_BKIN_PORT,ADVANCEDTIM_BKIN_PIN);	
}

static void AdvancedTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	
	//�򿪻�����ʱ����ʱ��
	ADVANCEDTIM_CLK_FUNC(ADVANCEDTIM_CLK, ENABLE);
	
	/* ��ʱ�������ṹ�� */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ��˳�Ա����ͨ�ú͸߼���ʱ�����ⲿ����ʱ��ʱ�������˲��ķ�Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ADVANCEDTIM_PERIOD;
	TIM_TimeBaseInitStruct.TIM_Prescaler = ADVANCEDTIM_PRESCALER;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;  //��ʹ���ظ�����
	TIM_TimeBaseInit(ADVANCEDTIMx, &TIM_TimeBaseInitStruct);
	
	/* ��ʱ������ȽϽṹ�� */
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;  //����������OC1�ĵ�ƽΪ�ߵ�ƽ
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  //����������OC1N�ĵ�ƽΪ�͵�ƽ
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;  //OC1N�͵�ƽ��Ч
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;  //OC1�ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;  //����OC1N�ź��������Ӧ���������
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;   //����OC1�ź��������Ӧ���������
	TIM_OCInitStruct.TIM_Pulse = ADVANCEDTIM_PULSE;  //ռ�ձ�
	TIM_OC1Init(ADVANCEDTIMx, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(ADVANCEDTIMx, TIM_OCPreload_Enable);
	
	/* ��ʱ��������ɲ���ṹ�� */
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;
	// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
	// �������õ�����ʱ��Ϊ152ns
	TIM_BDTRInitStruct.TIM_DeadTime = 11; //����ʱ��
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;  //����ģʽ�£���ʱ��������ʱ��OC/OCN����������е�ƽ��Ȼ��ʹ������ź�=1
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;  //����ģʽ�£���ʱ��������ʱ��OC/OCN����������е�ƽ��Ȼ��ʹ������ź�=1
	TIM_BDTRConfig(ADVANCEDTIMx, &TIM_BDTRInitStruct);
	
	TIM_Cmd(ADVANCEDTIMx, ENABLE);	
	TIM_CtrlPWMOutputs(ADVANCEDTIMx, ENABLE); //�����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
}

void AdvancedTim_Init(void)
{
	GPIO_Config();
	AdvancedTim_Config();
}
