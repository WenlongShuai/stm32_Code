#ifndef __BSP_USART1_5_H__
#define __BSP_USART1_5_H__

#include "stm32f10x.h"

#define USARTx										 USART1
#define USART_TX_PORT					  	 GPIOA
#define USART_TX_PIN 							 GPIO_Pin_9
#define USART_RX_PORT 						 GPIOA
#define USART_RX_PIN 							 GPIO_Pin_10
#define UASRT_GPIO_CLK 						 RCC_APB2Periph_GPIOA
																	 
#define USART_CLK 								 RCC_APB2Periph_USART1
#define USART_BAUDRATE 						 115200
#define USART_WORDLENGTH 					 USART_WordLength_8b
#define USART_PARITY 							 USART_Parity_No
#define USART_STOPBITS 						 USART_StopBits_1
#define USART_MODE 								 (USART_Mode_Rx | USART_Mode_Tx)
#define USART_HARDWAREFLOWCONTROL  USART_HardwareFlowControl_None

#define USART_NVIC_IRQ  					 USART1_IRQn
#define USART_NVIC_PREEPRIORITY 	 1
#define USART_NVIC_SUBPRIORITY 		 1
#define USART_NVIC_PRIORITYGROUP 	 NVIC_PriorityGroup_1

void USART_GPIO_Init();
void Usart_SendByteData(USART_TypeDef* USART, uint8_t sendData);
void Usart_SendHalfWordData(USART_TypeDef* USART, uint16_t sendData);
void Usart_SendArrayData(USART_TypeDef* USART, uint8_t *sendData, uint8_t arrayLen);
void Usart_SendStrData(USART_TypeDef* USART, uint8_t *sendData);


#endif /* __BSP_USART1_5_H__ */

