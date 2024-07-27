#include "stm32f10x.h"
#include "bsp_wwdg.h"
#include "bsp_led.h"
#include "bsp_usart.h"

/*
stm32���ڿ��Ź�����Ʊ�������һ��ʱ���Ϩ�����֮����һֱû��������˵��ϵͳû�в�����λ��
���������λ���ƻᱻ�������жϷ������е�����û�б�������˵��ι��������
������Ҫ���ĳ���ִ�е�ʱ��Ϊ20ms
*/

void delay(uint32_t time);

int main()
{
	uint8_t wwdg_wr = 0;
	uint8_t wwdg_tr = 0;
	uint32_t i = 0;
	
	LED_GPIO_Config();
	USART_GPIO_Init();
	
	LED_R(ON);
	
	delay(0x5FFFFF);
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		RCC_ClearFlag();
		printf("���ڿ��Ź������ĸ�λ\r\n");
	}
	else
	{
		printf("�ϵ縴λ���߰�����λ\r\n");
	}
	
	//T = Tpclk1 * 4096 * 2^WGDTB
	//WGDTB = 0 --> T = 113us
	//WGDTB = 1 --> T = 227us
	//WGDTB = 2 --> T = 455us
	//WGDTB = 3 --> T = 910us
	//���ڿ��Ź��Ĵ������޵�ʱ����΢С�ڼ�����ִ�е�ʱ�䣬(TR-WR)*T < 20ms��TR = 0x7F,WR < 0x7F
	WWDG_Config(WWDG_Prescaler_1, 0x55, 0x7F);
	
	wwdg_wr = WWDG->CFR & 0x7F;   //ȡ���������޵�ֵ
	
	LED_R(OFF);
	
	while(1)
	{
		/*-------------------------------------------*/
		
		/* �ⲿ��Ӧ��д��Ҫ��WWDG���ĳ�����γ��������ʱ������˴���ֵӦ�����óɶ�� */
		
		/*-------------------------------------------*/
		
		wwdg_tr = WWDG->CR & 0x7F;  //ȡ����ǰ��������ֵ
		if(wwdg_tr < wwdg_wr)
		{
			//����������ֵС�ڴ�������ֵʱ����ִ��ι������
			WWDG_Feed();
		}
		
	}
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

