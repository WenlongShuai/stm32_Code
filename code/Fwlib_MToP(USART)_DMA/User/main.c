#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma_MToP.h"

extern uint8_t memoryData[BUFFER_SIZE];

void Delay(uint32_t time);

int main()
{
	uint16_t i = 0;
	
	for(i = 0;i < BUFFER_SIZE;i++)
	{
		memoryData[i] = 'S';
	}
	
	LED_GPIO_Config();
	USART_DMA_Config();
	MToP_DMA_Config();

	//USART1��DMA���� ���ڷ��� ����
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);
	
	
	//DMA ��USART1�������ݵ�ʱ�򣬲���ռ��CPU����ʱCPU���Լ������Լ������飬�����˶��߳�һ��
	while(1)
	{
		LED_R_TOGGLE;
		Delay(0xFFFFF);
	}
	return 0;
}

void Delay(uint32_t time)
{
	while(time--);
}

