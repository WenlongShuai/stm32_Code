#include "stm32f10x.h"

//����RGB�Ƶ�����һ����ɫ

#define GREEN_LED 0
#define RED_LED   0
#define BLUE_LED  0

int main()
{
	//����ODR�Ĵ�������LED
	
	#if GREEN_LED
	//����PB0����
	//��GPIOB��ʱ��
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	//����GPIOB���������ģʽ
	*(unsigned int*)0x40010C00 |= ( 1 << (4*0) );
	//����GPIOB����ߵ͵�ƽ
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<0);
	#endif
	
	#if RED_LED
	//����PB5����
	//��GPIOB��ʱ��
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	
	//����GPIOB���������ģʽ
	*(unsigned int*)0x40010C00 |= ( 1 << (4*5) );
	
	//����GPIOB����ߵ͵�ƽ
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<5);
	
	#endif
	
	#if BLUE_LED
	//����PB1����
	//��GPIOB��ʱ��
	*(unsigned int*)(0x40021000+0x18) |= (1<<3);
	
	//����GPIOB���������ģʽ
	*(unsigned int*)0x40010C00 |= ( 1 << (4*1) );
	
	//����GPIOB����ߵ͵�ƽ
	*(unsigned int*)(0x40010C00+0x0C) &= ~(1<<1);
	
	#endif
	
	#if 0
	//����BSRR�Ĵ�������LED
	//��GPIOB��ʱ��
	RCC_APB2ENR |= (1<<3);
	//����GPIOB���������ģʽ
	GPIOB->CRL &= ~(0xf<<(4*0));
	GPIOB->CRL |= (0x1<<(4*0));
	//����BSRR�Ĵ������ƶ˿�����ߵ͵�ƽ
	//GPIOB->BSRR |= (1<<16);  //��LED��
	GPIOB->BSRR |= (1<<0);   //�ر�LED��
	
	#else
	//����BRR�Ĵ�������LED
	//��GPIOB��ʱ��
	RCC_APB2ENR |= (1<<3);
	//����GPIOB���������ģʽ
	GPIOB->CRL &= ~(0xf<<(4*1));
	GPIOB->CRL |= (0x1<<(4*1));
	//����BSRR�Ĵ������ƶ˿�����͵�ƽ
	GPIOB->BRR |= (1<<1);  //BRR����Ĵ���ֻ�����ö˿����0���������ö˿����1
	
	#endif

	return 0;
}

//��Ϊ�����ļ���ȥ�����ⲿ��SystemInit������Ϊ�˲��ñ��������������ȶ���һ���յ�SystemInit����
void SystemInit()
{

}
