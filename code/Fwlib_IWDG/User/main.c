#include "stm32f10x.h"
#include "bsp_iwdg.h"
#include "bsp_led.h"
#include "bsp_key.h"

/*
stm32�������Ź������Ź����ʱ��Ϊ1S��ͨ��IWDG_Config�����Ĳ����������������
����ǿ��Ź���λ������ƣ��ϵ縴λ���߰�����λ�������ơ�
����ڿ��Ź����ʱ�䷶Χ֮�ڽ���ι�����ͻ����̵ơ�
*/

void delay(uint32_t time);

int main()
{
	LED_GPIO_Config();
	Key_GPIO_Config();
	
	delay(0x5FFFFF);
	
	//���Ź���λ
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		RCC_ClearFlag();  //�����λ��־
		LED_R(ON);	
	}
	else
	{
		//���Ƕ������Ź���λ���������ϵ縴λ���߰�����λ
		LED_B(ON);
	}
	
	//�򿪿��Ź�����
	IWDG_Config(IWDG_Prescaler_16, 0x9C4);
	
	while(1)
	{
		//ͨ��KEY1����ι��
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON)
		{
			IWDG_Feed();
			LED_G(ON);
		}
	}
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

