#include "stm32f10x_gpio.h"

//GPIO��λ����������BSRR
void GPIO_SetBits(GPIO_Typedef *GPIOx, unsigned short GPIO_Pin)
{
	GPIOx->BSRR |= GPIO_Pin;
}

//GPIO��λ����������BRR
void GPIO_ResetBits(GPIO_Typedef *GPIOx, unsigned short GPIO_Pin)
{
	GPIOx->BRR |= GPIO_Pin;
}

//GPIO��ʼ������������CRL
void GPIO_Init(GPIO_Typedef *GPIOx, GPIO_InitStructure_Typedef *GPIO_Struct)
{
	unsigned char speed = 0x00;
	unsigned int tmpCRL = 0x00, tmpCRH = 0x00;
	unsigned char pin = 0;
	unsigned int mode = 0x00;
	unsigned char i = 0;
	
	//�ж�GPIO��ģʽ������ģʽ�������ģʽ���ж�GPIO_Mode��bit4��0����1
	if(((GPIO_Struct->GPIO_Mode) & 0x10) != 0x00)
	{
		speed = GPIO_Struct->GPIO_Speed;
	}
	
	//�ж�GPIO_Pinx�Ƿ�Ϊ��8λ
	if((GPIO_Struct->GPIO_Pinx & 0xFF00) == 0)
	{
		tmpCRL = GPIOx->CRL;
		//ȷ��GPIO���������
		for(i = 0;i < 8;i++)
		{
			if((GPIO_Struct->GPIO_Pinx & (1<<i)) == GPIO_Struct->GPIO_Pinx)
			{
				pin = i;
				break;
			}
		}
		
		//��CRL�������
		tmpCRL &= ~(0x0f<<(4*pin));
		
		//����CRL��ֵ
		mode = GPIO_Struct->GPIO_Mode & 0x0F;
		tmpCRL |= ((mode | speed) << (4*pin));
		
		if(GPIO_Struct->GPIO_Mode == GPIO_Mode_IPD)
		{
			GPIOx->BRR |= (1<<pin);
		}
		else
		{
			GPIOx->BSRR |= (1<<pin);
		}
		
		GPIOx->CRL = tmpCRL;
	}
	else
	{
		tmpCRH = GPIOx->CRH;
		//ȷ��GPIO���������
		for(i = 8;i < 16;i++)
		{
			if(GPIO_Struct->GPIO_Pinx & (1<<i) != 0x00)
			{
				pin = i;
				break;
			}
		}
		
		//��CRH�������
		tmpCRH &= ~(0x0f<<(4*pin));
		
		//����CRL��ֵ
		mode = GPIO_Struct->GPIO_Mode & 0x0F;
		tmpCRH |= (mode | speed) << (4*pin);
		
		if(GPIO_Struct->GPIO_Mode == GPIO_Mode_IPD)
		{
			GPIOx->BRR |= (1<<pin);
		}
		else
		{
			GPIOx->BSRR |= (1<<pin);
		}
		
		GPIOx->CRH = tmpCRH;
	}
}

