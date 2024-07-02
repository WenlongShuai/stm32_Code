#include "bsp_gpio_exti.h"


static void KEY_NVIC_Config()
{
	NVIC_InitTypeDef NVICInitStruct;
	
	//KEY1
	NVICInitStruct.NVIC_IRQChannel = KEY1_NVIC_IRQCHANNEL;
	NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = KEY1_NVIC_IRQCHANNEL_PREPRIORITY;  //�����ȼ�
	NVICInitStruct.NVIC_IRQChannelSubPriority = KEY1_NVIC_IRQCHANNEL_SUBPRIORITY;    //�����ȼ�
	NVIC_PriorityGroupConfig(KEY1_NVIC_PRIORITY_GROUP);  //�����ж����ȼ�����
	NVIC_Init(&NVICInitStruct);
	
	//KEY2
	NVICInitStruct.NVIC_IRQChannel = KEY2_NVIC_IRQCHANNEL;
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = KEY2_NVIC_IRQCHANNEL_PREPRIORITY;  //�����ȼ�
	NVICInitStruct.NVIC_IRQChannelSubPriority = KEY2_NVIC_IRQCHANNEL_SUBPRIORITY;    //�����ȼ�
	NVIC_PriorityGroupConfig(KEY2_NVIC_PRIORITY_GROUP);  //�����ж����ȼ�����
	NVIC_Init(&NVICInitStruct);
}


static void KEY_GPIO_Config()
{
	RCC_APB2PeriphClockCmd(KEY1_GPIO_PORT_EXTI, ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_GPIO_PORT_EXTI, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//KEY1
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN_EXTI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT_EXTI, &GPIO_InitStruct);
	
	//KEY2
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN_EXTI;
	GPIO_Init(KEY2_GPIO_PORT_EXTI, &GPIO_InitStruct);
}

void KEY_EXTI_Config()
{
	EXTI_InitTypeDef EXTIInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //��AFIO��ʱ��
	
	//KEY1
	EXTIInitStruct.EXTI_Line = KEY1_EXTI_LINE;
	EXTIInitStruct.EXTI_Mode = KEY1_EXTI_MODE;
	EXTIInitStruct.EXTI_Trigger = KEY1_EXTI_TRIGGER;
	EXTIInitStruct.EXTI_LineCmd = ENABLE;
	GPIO_EXTILineConfig(KEY1_EXTI_LINE_GPIO_PORT, KEY1_EXTI_LINE_GPIO_PIN);  //����������AFIOΪPA0����
	EXTI_Init(&EXTIInitStruct); //ʹ���ж�����(����ѡ��Ĵ���������ж��¼����������������Ĵ������ж����μĴ���)

	//KEY2
	EXTIInitStruct.EXTI_Line = KEY2_EXTI_LINE;
	EXTIInitStruct.EXTI_Mode = KEY2_EXTI_MODE;
	EXTIInitStruct.EXTI_Trigger = KEY2_EXTI_TRIGGER;
	GPIO_EXTILineConfig(KEY2_EXTI_LINE_GPIO_PORT, KEY2_EXTI_LINE_GPIO_PIN);  //����������AFIOΪPA0����
	EXTI_Init(&EXTIInitStruct); //ʹ���ж�����(����ѡ��Ĵ���������ж��¼����������������Ĵ������ж����μĴ���)
	
	KEY_GPIO_Config();
	KEY_NVIC_Config();
}



