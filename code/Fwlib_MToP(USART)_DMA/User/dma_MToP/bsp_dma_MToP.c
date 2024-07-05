#include "bsp_dma_MToP.h"

uint8_t memoryData[BUFFER_SIZE] = {0};

static void USART_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��PA9��PA10��ʱ��
	RCC_APB2PeriphClockCmd(USART_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = USART_GPIO_TX_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(USART_GPIO_TX_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = USART_GPIO_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(USART_GPIO_RX_PORT, &GPIO_InitStruct);
}

void USART_DMA_Config()
{
	USART_InitTypeDef USART_InitStruct;
	//��USART1��ʱ��
	RCC_APB2PeriphClockCmd(USART_CLK, ENABLE);
	
	USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
	USART_InitStruct.USART_HardwareFlowControl = USART_HARDWAREFLOWCTRL;
	USART_InitStruct.USART_Mode = USART_MODE;
	USART_InitStruct.USART_Parity = USART_PARITY;
	USART_InitStruct.USART_StopBits = USART_STOPBITS;
	USART_InitStruct.USART_WordLength = USART_WORDLENGTH;
	
	USART_Init(DEBUG_USART, &USART_InitStruct);
	
	
	USART_Cmd(DEBUG_USART, ENABLE);
	
	USART_GPIO_Config();
}

/* ����DMA�Ĺ���ģʽΪ M--->P */
void MToP_DMA_Config()
{
	DMA_InitTypeDef MToM_DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(DMA_CLK, ENABLE);
	
	/* DMA�����ݴ���������������ȥ */
 	MToM_DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(USART1_BASE+0x04);
	MToM_DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)memoryData;
	MToM_DMA_InitStruct.DMA_DIR = DMA_PERIPHERAL_DIR;
	
	/* DMAһ���Դ��������ݣ����ݵĿ�� */
	MToM_DMA_InitStruct.DMA_BufferSize = DMA_BUFFERSIZE;
	MToM_DMA_InitStruct.DMA_PeripheralInc = DMA_PERIPHERAL_INC;
	MToM_DMA_InitStruct.DMA_PeripheralDataSize = DMA_PERIPHERAL_DATASIZE;
	MToM_DMA_InitStruct.DMA_MemoryInc = DMA_MEMORY_INC;
	MToM_DMA_InitStruct.DMA_MemoryDataSize = DMA_MEMORY_DATASIZE;
	
	/* DMA ����ʲôʱ������� */
	MToM_DMA_InitStruct.DMA_Mode = DMA_MODE;  //����������ɽ���������Ҫѭ������
	
	MToM_DMA_InitStruct.DMA_M2M = DMA_MToM;  //DMA�Ƿ�ΪMToM
	MToM_DMA_InitStruct.DMA_Priority = DMA_PRIORITY;  //DMA�����ȼ�
	
	DMA_Init(DMA_CHANNEL, &MToM_DMA_InitStruct);
	
	DMA_ClearFlag(DMA_TC_FLAG);  //Ϊ�˱�֤������ɲ������ţ������ڳ�ʼ��ʱ�������ɱ�־λ
	
	DMA_Cmd(DMA_CHANNEL, ENABLE);
}

void Usart_SendByteData(USART_TypeDef* USART, uint8_t sendData)
{
	USART_SendData(USART, sendData);
	while(USART_GetFlagStatus(USART, USART_FLAG_TXE) != SET);
}


