#include "stm32f10x.h"

//����RGB�Ƶ�����һ����ɫ

#define GREEN_LED 0
#define RED_LED   0
#define BLUE_LED  1

int main()
{
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

	return 0;
}

//��Ϊ�����ļ���ȥ�����ⲿ��SystemInit������Ϊ�˲��ñ��������������ȶ���һ���յ�SystemInit����
void SystemInit()
{

}
