#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_CANBus.h"
#include "bsp_key.h"
#include "string.h"

/* ��ʵ���Ƕ� CAN���� �Ļ���ģʽ���в��ԣ���Ϊ CAN���� ������ģʽ��Ҫ���һ��CAN�շ���ģ����ʵ�� */

extern CanTxMsg can_Tx_Data;
extern CanRxMsg can_Rx_Data;
extern uint8_t receiveFlag;

int main()
{
	uint8_t mailBox = 0;
	uint8_t data[8] = {'0',101,102,0,104,5,6,7};
	uint8_t i = 0;

	USART_GPIO_Init();
	LED_GPIO_Config();
	Key_GPIO_Config();
	CANx_Init();
	
	LED_B(ON);
	printf("����һ�� CAN���� ͨѶʵ��\r\n");
	printf("���� KEY1 �������Ϳ���ͨ��CAN���߷�������\r\n");
	
	can_Tx_Data.StdId = 0;
	can_Tx_Data.ExtId = CAN_FILTER_PASSID;
	can_Tx_Data.DLC = 8;
	//strcpy������������'\0'��ֹͣcopy������data�����о�����Ҫ����0��
	//����copy��can_Tx_Data.Data�����ݾ������⣬����CAN���߷��͵����ݾ������⡣0����'\0'��
	strcpy((char *)can_Tx_Data.Data,(const char*)data); 
	can_Tx_Data.IDE = CAN_Id_Extended;
	can_Tx_Data.RTR = CAN_RTR_Data;
	
	
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			mailBox = CAN_Transmit(CAN_x, &can_Tx_Data);
			
			while(CAN_TransmitStatus(CAN_x, mailBox) != CAN_TxStatus_Ok);
			
			printf("���ݰ��������\r\n");
		}
		
		if(receiveFlag == 1)
		{
			receiveFlag = 0;
			printf("���յ�������Ϊ:");
			for(i = 0;i < 8;i++)
			{
				printf("%d  ",can_Rx_Data.Data[i]);
			}
			printf("\r\n");
		}
	}
	
	return 0;
}

