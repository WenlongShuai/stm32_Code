#include "stm32f10x.h"
#include "bsp_usart1_5.h"


int main()
{
	uint8_t arry[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
	uint8_t *str = "使用STM32F103开发板\n";
	
	USART_GPIO_Init();
	
	//Usart_SendByteData(USARTx, 100);
	//Usart_SendHalfWordData(USARTx, 0xffaa);
	
	Usart_SendArrayData(USARTx, arry, sizeof(arry)/sizeof(arry[0]));
	
	Usart_SendStrData(USARTx, str);
	
	
	
	
	while(1)
	{
		
	}
	
	return 0;
}

