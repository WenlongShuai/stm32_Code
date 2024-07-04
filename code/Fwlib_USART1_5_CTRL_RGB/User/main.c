#include "stm32f10x.h"
#include "bsp_usart1_5.h"
#include "bsp_led.h"

int main()
{
	uint8_t *str = "\n\
����ĿΪ��λ������STM32F103�������RGB\n\
---------------1:�������------------\n\
---------------2:�����̵�------------\n\
---------------3:��������------------\n\
---------------4:�ر�RGB-------------\n\
---------------0:�˳�����------------\n";
	
	USART_GPIO_Init();
	LED_GPIO_Config();
	
	Usart_SendStrData(USARTx, str);
	
	do
	{
		switch(receiveData)
		{
			case '1':
				LED_R(ON);
				break;
			case '2':
				LED_G(ON);
				break;
			case '3':
				LED_B(ON);
				break;
			case '4':
				LED_BGR_OFF;
				break;
		}
	}while(receiveData != '0');
	
	Usart_SendStrData(USARTx, "�˳��ɹ�");
	
	return 0;
}

