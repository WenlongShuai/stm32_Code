#include "bsp_wwdg.h"

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Init(&NVIC_InitStruct);
}

void WWDG_Config(uint8_t prv, uint8_t wr, uint8_t tr)
{
	//�򿪴��ڿ��Ź���ʱ��PCLK1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	//����WWDG������ֵ
	WWDG_SetCounter(tr);
	
	//���ô��ڿ��Ź���Ԥ��Ƶʱ��
	WWDG_SetPrescaler(prv);
	
	//���ô��ڿ��Ź��Ĵ�������
	WWDG_SetWindowValue(wr);
	
	//ʹ�ܴ��ڿ��Ź��������ô��ڿ��Ź���������ֵΪ0x7F
	//����WWDG�����ؼ�����ֵ
	WWDG_Enable(WWDG_CNT);
	
	//�����ǰ�����жϱ�־
	WWDG_ClearFlag();
	
	//����WWDG���ڿ��Ź����ж����ȼ�
	NVIC_Config();
	
	//����ǰ�����ж�
	WWDG_EnableIT();
}

void WWDG_Feed(void)
{
	//ˢ�µݼ���������ֵ��ι�����������ó�����WWDG_CNT=0x7F
	WWDG_SetCounter(WWDG_CNT);
}
