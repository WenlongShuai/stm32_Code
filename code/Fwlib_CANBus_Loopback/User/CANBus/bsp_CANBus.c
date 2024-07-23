#include "bsp_CANBus.h"

CanTxMsg can_Tx_Data = {0};
CanRxMsg can_Rx_Data = {0};

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = CAN_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_Init(&NVIC_InitStruct);
	
}

/* ���stm32��bxCAN����� CAN�շ�������Ҫͨ������ĺ�����ʼ��CAN�շ�����Ӧ��GPIO */
/* ���û��CAN�շ����Ͳ�Ҫʹ�øú��� */
static void GPIO_Config()
{
	/* CAN�շ��� GPIO ��صĳ�ʼ�� */
	GPIO_InitTypeDef GPIO_InitStruct;
	
	CAN_GPIO_CLK_FUNC(CAN_GPIO_CLK, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = CAN_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = CAN_RX_GPIO_PIN;
	GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStruct);
}


static void CAN_Config(void)
{
	CAN_InitTypeDef CAN_InitStruct;   //CAN �Ļ�����ʼ���ṹ��
	CAN_FilterInitTypeDef CAN_FilterInitStruct;  //CAN �Ĺ�������ʼ���ṹ��
	
	//��CAN�����ʱ��
	CAN_CLK_FUNC(CAN_CLK, ENABLE);
	CAN_InitStruct.CAN_Mode = CAN_Mode_LoopBack;  //����CAN�Ĳ���ģʽ���˴�����Ϊ����ģʽ����Ϊ������û�����CAN�շ���
	CAN_InitStruct.CAN_ABOM = ENABLE;  //�Զ����߹رչ�������
	CAN_InitStruct.CAN_AWUM = ENABLE;  //�Զ���������
	CAN_InitStruct.CAN_NART = DISABLE;  //���Զ��ش�ģʽ����
	CAN_InitStruct.CAN_Prescaler = 4;
	CAN_InitStruct.CAN_RFLM = ENABLE;   //����FIFO����ģʽ����
	CAN_InitStruct.CAN_TTCM = DISABLE;   //ʱ�䴥��ͨ��ģʽ����
	CAN_InitStruct.CAN_TXFP = DISABLE;   //����FIFO���ȼ�����
	CAN_InitStruct.CAN_BS1 = CAN_BS1_5tq;  //(4+1) 
	CAN_InitStruct.CAN_BS2 = CAN_BS2_3tq;	 //(2+1)
	CAN_InitStruct.CAN_SJW = CAN_SJW_2tq;  //(1+1)
	
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask; //��ʶ������  //CAN_FilterMode_IdList;  //��ʶ���б�
	CAN_FilterInitStruct.CAN_FilterNumber = 0;  //ɸѡ��ѡ��0~13�飨ÿ��2��32λ�Ĵ�����
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = (((CAN_FILTER_PASSID << 3)|CAN_Id_Extended|CAN_RTR_DATA)&0xFFFF0000) >> 16;
	CAN_FilterInitStruct.CAN_FilterIdLow = ((CAN_FILTER_PASSID << 3)|CAN_Id_Extended|CAN_RTR_DATA)&0xFFFF;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFFF; //��ʶ����չȫ��ƥ��
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0xFFFF;
	
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_Init(CAN_x, &CAN_InitStruct);
	
	CAN_ITConfig(CAN_x, CAN_IT_FMP0, ENABLE);
	
	CAN_ClearITPendingBit(CAN_x, CAN_IT_FMP0);
}


void CANx_Init(void)
{
	NVIC_Config();
	GPIO_Config();
	CAN_Config();
}

