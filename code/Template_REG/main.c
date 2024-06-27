#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define GPIO_CLOCK_ENABLE (RCC->APB2ENR |= (1<<3))
#define GPIO_PIN          GPIO_Pin_5
#define GPIO_PORT         GPIOB

int main()
{
	#if 0
	//ʹ���������ַ���ʼĴ���
	//����GPIOB��ʱ��
	RCC_APB2ENR |= (1<<3);
	//����GPIOB���������ģʽ
	GPIOB_CRL &= ~(0xf<<(4*5));  //��CRL�Ĵ����е�PB5����
	GPIOB_CRL |= (1<<(4*5));  //��CRL�Ĵ����е�PB5����Ϊ0001
	//����GPIOB����ߵ͵�ƽ
	GPIOB_ODR &= ~(1<<5);
	
	#elif 0
	
	//ʹ�ýṹ���������Ĵ���
	//����GPIOB��ʱ��
	RCC->APB2ENR |= (1<<3);
	//����GPIOB���������ģʽ
	GPIOB->CRL &= ~(0xf<<(4*5));  //��CRL�Ĵ����е�PB5����
	GPIOB->CRL |= (1<<(4*5));  //��CRL�Ĵ����е�PB5����Ϊ0001
	//����GPIOB����ߵ͵�ƽ
	GPIOB->ODR &= ~(1<<5);
	
	#elif 0
	
	//��߿ɶ���
	GPIO_InitStructure_Typedef GPIO_Struct;
	//����GPIOB��ʱ��
	RCC->APB2ENR |= (1<<3);
	
	//RED
	//����GPIOB���������ģʽ
	GPIO_Struct.GPIO_Pinx = GPIO_Pin_5;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHZ;
	
	GPIO_Init(GPIOB, &GPIO_Struct);
	//ʹ��BRR�Ĵ�������͵�ƽ
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	//ʹ��BSRR�Ĵ�������ߵ�ƽ
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	
	//GREEN
	GPIO_Struct.GPIO_Pinx = GPIO_Pin_0;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHZ;
	
	GPIO_Init(GPIOB, &GPIO_Struct);
	//ʹ��BRR�Ĵ�������͵�ƽ
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	//ʹ��BSRR�Ĵ�������ߵ�ƽ
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	
	//BLUE
	GPIO_Struct.GPIO_Pinx = GPIO_Pin_1;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHZ;
	
	GPIO_Init(GPIOB, &GPIO_Struct);
	//ʹ��BRR�Ĵ�������͵�ƽ
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	//ʹ��BSRR�Ĵ�������ߵ�ƽ
	//GPIO_SetBits(GPIOB, GPIO_Pin_1);
	
	#elif 1
	
	//�����ֲ��
	GPIO_InitStructure_Typedef GPIO_Struct;
	//����GPIOB��ʱ��
	GPIO_CLOCK_ENABLE;
	
	//RED
	//����GPIOB���������ģʽ
	GPIO_Struct.GPIO_Pinx = GPIO_PIN;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHZ;
	
	GPIO_Init(GPIO_PORT, &GPIO_Struct);
	//ʹ��BRR�Ĵ�������͵�ƽ
	GPIO_ResetBits(GPIO_PORT, GPIO_PIN);
	//ʹ��BSRR�Ĵ�������ߵ�ƽ
	//GPIO_SetBits(GPIO_PORT, GPIO_PIN);
	
	
	
	#endif
	
	return 0;
}

//��Ϊ�����ļ���ȥ�����ⲿ��SystemInit������Ϊ�˲��ñ��������������ȶ���һ���յ�SystemInit����
void SystemInit()
{

}
