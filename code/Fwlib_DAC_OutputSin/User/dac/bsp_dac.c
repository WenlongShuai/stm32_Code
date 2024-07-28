#include "bsp_dac.h"

const uint16_t sinData12bit[] = {2048, 2460, 2856, 3218, 3532, 3786, 3969, 4072, 
														4093, 4031, 3887, 3668, 3382, 3042, 2661, 2255, 
														1841, 1435, 1054, 714, 428, 209, 65, 3, 
														24, 127, 310, 564, 878, 1240, 1636, 2048};

uint32_t dualSinData12bit[32] = {0};
	
static void DAC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	//��DMAʱ��
	DAC_DMA_CLK_FUNC(DAC_DMA_CLK, ENABLE);
	
	DMA_InitStruct.DMA_BufferSize = sizeof(sinData12bit)/sizeof(sinData12bit[0]);
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)dualSinData12bit;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)DAC->DHR12RD;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DAC_DMA_CHANNELx, &DMA_InitStruct);
	DMA_Cmd(DAC_DMA_CHANNELx, ENABLE);
}



static void DAC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeInitStruct;
	
	//��TIM��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ���ӣ�����tDTS����
	TIM_TimeInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeInitStruct.TIM_Period = (20-1);  //��ʱ����Ϊ20
	TIM_TimeInitStruct.TIM_Prescaler = 0x0;
	TIM_TimeInitStruct.TIM_RepetitionCounter = 0; //�ظ���������TIM1��TIM8��ʹ��
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeInitStruct);
	
	//ѡ��TIMx���������ģʽ����ΪDAC�Ĵ���Դ
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	
	//ʹ��TIM
	TIM_Cmd(TIM2, ENABLE);
}

static void DAC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��GPIOʱ��
	DAC_GPIO_CLK_FUNC(DAC_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = DAC_GPIO_PIN;
	GPIO_Init(DAC_GPIO_PORT, &GPIO_InitStruct);
}

static void DAC_Config(void)
{
	DAC_InitTypeDef DAC_InitStruct;
	
	//��DAC����ʱ��
	DAC_CLK_FUNC(DAC_CLK, ENABLE);
	
	//����DAC1
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;  //����/���ǲ��Ƿ�ʹ��
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;  //��ѡ����Ҫ��DAC_WaveGenerationʹ�ܵ�����²���Ч
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;  //ʹ��������壬���Լ�С����迹
	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	//ʹ��DAC
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	//��Ϊʹ��˫ͨ��DAC��������ͬ�������� ����DAC2
	DAC_Init(DAC_Channel_2, &DAC_InitStruct);
	//ʹ��DAC
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	//ʹ��DMA��ʹ��DMA����DAC1������ʱ��ͬʱ����DAC2��ֵ
	DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void DACx_Init(void)
{
	uint8_t i = 0;
	DAC_GPIO_Config();
	DAC_TIM_Config();
	DAC_Config();
	
	//����ͨ�����Ҳ�������䵽��ͨ���Ҷ���
	for(i = 0;i < 32;i++)
	{
		dualSinData12bit[i] = (sinData12bit[i] << 16) | sinData12bit[i];
	}
	
	DAC_DMA_Config();
}

