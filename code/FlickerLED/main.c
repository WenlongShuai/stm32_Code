#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char

void delayMs(uint ms);

int main()
{
	//����PB0����
	//��GPIOB��ʱ��
	*(uint*)(0x40021000+0x18) |= (1<<3);
	//����GPIOB���������ģʽ
	*(uint*)0x40010C00 |= ( 1 << (4*0) );
	
	while(1)
	{
		//����GPIOB����ߵ͵�ƽ
		*(uint*)(0x40010C00+0x0C) &= ~(1<<0);  //��
		delayMs(1000);
		*(uint*)(0x40010C00+0x0C) |= (1<<0);  //��
		delayMs(1000);
	}
	
	
	return 0;
}

//��ʱ����
void delayMs(uint ms)
{
	uint i = 0;
	uint j = 0;
	for(i = 0;i < ms;i++)
		for(j = 0;j < 2003;j++);
}


//��Ϊ�����ļ���ȥ�����ⲿ��SystemInit������Ϊ�˲��ñ��������������ȶ���һ���յ�SystemInit����
void SystemInit()
{

}


