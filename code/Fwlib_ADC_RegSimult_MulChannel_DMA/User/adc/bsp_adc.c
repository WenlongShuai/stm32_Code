#include "bsp_adc.h"

uint32_t adcConversionValue[ADCx_1_2_CHANNEL_NUM] = {0};

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��ADC��GPIOʱ��
	ADCx_GPIO_CLK_FUNC(ADCx_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL10_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL10_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL11_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL11_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL12_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL12_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL13_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL13_GPIO_PORT, &GPIO_InitStruct);
	
	#if ADC_1 || ADC_2
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL14_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL14_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ADCx_1_2_CHANNEL15_GPIO_PIN;
	GPIO_Init(ADCx_1_2_CHANNEL15_GPIO_PORT, &GPIO_InitStruct);
	#endif
}

static void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	//��ADC1����ʱ��
	ADCx_1_CLK_FUNC(ADCx_1_CLK, ENABLE);
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;  //ͬ������
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;  //һ��ͨ������Ҫɨ��
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel = ADCx_1_2_CHANNEL_NUM;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx_1, &ADC_InitStruct);
	
	
	//��ADC2����ʱ��
	ADCx_1_CLK_FUNC(ADCx_2_CLK, ENABLE);
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;  //ͬ������
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;  //һ��ͨ������Ҫɨ��
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_NbrOfChannel = ADCx_1_2_CHANNEL_NUM;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx_2, &ADC_InitStruct);
	
	
	//����ADC�����ADCCLKʱ��(APB2��8��Ƶ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//ADC1��ͨ������(����ʱ�䡢Rank(����channel�ڵڼ���ת��))
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_2_CHANNEL11, 1, ADCx_1_2_SAMPLETIME);
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_2_CHANNEL12, 2, ADCx_1_2_SAMPLETIME);
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_2_CHANNEL13, 3, ADCx_1_2_SAMPLETIME);
	
	//ADC2��ͨ������(����ʱ�䡢Rank(����channel�ڵڼ���ת��))
	ADC_RegularChannelConfig(ADCx_2, ADCx_1_2_CHANNEL14, 3, ADCx_1_2_SAMPLETIME);
	ADC_RegularChannelConfig(ADCx_2, ADCx_1_2_CHANNEL15, 2, ADCx_1_2_SAMPLETIME);
	ADC_RegularChannelConfig(ADCx_2, ADCx_1_2_CHANNEL13, 1, ADCx_1_2_SAMPLETIME);
	
	//��DMA����,��Ϊֻ��ADC1��ADC3����DMA
	ADC_DMACmd(ADCx_1, ENABLE);
	
	//ʹ��ADC1
	ADC_Cmd(ADCx_1, ENABLE);
	//ʹ��ADC2
	ADC_Cmd(ADCx_2, ENABLE);
	
	//ADC1У׼��ʼ��
	ADC_ResetCalibration(ADCx_1);
	while(ADC_GetResetCalibrationStatus(ADCx_1));
	
	//��ʼADC1У׼
	ADC_StartCalibration(ADCx_1);
	while(ADC_GetCalibrationStatus(ADCx_1));
	
	
	//ADC2У׼��ʼ��
	ADC_ResetCalibration(ADCx_2);
	while(ADC_GetResetCalibrationStatus(ADCx_2));
	
	//��ʼADC2У׼
	ADC_StartCalibration(ADCx_2);
	while(ADC_GetCalibrationStatus(ADCx_2));
	
	//ADC2ʹ���ⲿ�¼�����ת��
	ADC_ExternalTrigConvCmd(ADCx_2, ENABLE);
	
	//�������ADC1ת��
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);
}

//DMA
static void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	//��DMA����ʱ��
	DMAx_CLK_FUNC(DMAx_CLK, ENABLE);
	
	DMA_InitStruct.DMA_BufferSize = ADCx_1_2_CHANNEL_NUM;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adcConversionValue;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(ADCx_1->DR);
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMAx_CHANNELx, &DMA_InitStruct);
	DMA_Cmd(DMAx_CHANNELx, ENABLE);
}

void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	DMA_Config();
	ADCx_Config();
}
