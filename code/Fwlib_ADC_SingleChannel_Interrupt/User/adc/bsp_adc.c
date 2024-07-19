#include "bsp_adc.h"

uint16_t adcConversionValue = 0;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��ADC��GPIOʱ��
	ADCx_GPIO_CLK_FUNC(ADCx_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL10_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL10_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL11_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL11_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL12_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL12_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL13_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL13_GPIO_PORT, &GPIO_InitStruct);
	
	#if ADC_1 || ADC_2
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL14_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL14_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_CHANNEL15_GPIO_PIN;
	GPIO_Init(ADCx_CHANNEL15_GPIO_PORT, &GPIO_InitStruct);
	#endif
}

static void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	//��ADC����ʱ��
	ADCx_CLK_FUNC(ADCx_CLK, ENABLE);
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;  //һ��ͨ������Ҫɨ��
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel = ADCx_CHANNEL_NUM;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC_x, &ADC_InitStruct);
	
	//����ADC�����ADCCLKʱ��(APB2��8��Ƶ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//ADC��ͨ������(����ʱ�䡢Rank(����channel�ڵڼ���ת��))
	ADC_RegularChannelConfig(ADC_x, ADCx_CHANNEL11, 1, ADCx_SAMPLETIME);
	
	//��ADC�ж�
	ADC_ITConfig(ADC_x, ADCx_IT_FLAG, ENABLE);
	
	//ʹ��ADC
	ADC_Cmd(ADC_x, ENABLE);
	
	//ADCУ׼��ʼ��
	ADC_ResetCalibration(ADC_x);
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	//��ʼADCУ׼
	ADC_StartCalibration(ADC_x);
	while(ADC_GetCalibrationStatus(ADC_x));
	
	//�������ADCת��
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

//Ƕ�������жϿ�����
static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = ADCx_NVIC_IRQCHANNEL;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStruct);
}

void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	NVIC_Config();
	ADCx_Config();
}
