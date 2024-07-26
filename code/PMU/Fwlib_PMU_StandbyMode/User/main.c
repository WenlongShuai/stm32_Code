#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"


void delay(uint32_t time);

int main()
{
	USART_GPIO_Init();
	LED_GPIO_Config();
	
	//�ڽ������ģʽ֮ǰ�������ȴ�PWR��ʱ�ӣ�����ͽ����˴���ģʽ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	LED_B(ON);
	
	while(1)
	{
		if(Key2_LongPress())
		{
			LED_G(ON);
			printf("��������ģʽ���������ģʽ�����ʹ��KEY2���л��ѣ����Ѻ����и�λ�������ͷ��ʼִ��\r\n");
			delay(0xFFFFF);
			PWR_ClearFlag(PWR_FLAG_WU); //���WUF��־λ
			PWR_WakeUpPinCmd(ENABLE);  //ʹ��WKUP(PA0)���ţ�����ʹ��PA0��ʱ�򣬲���Ҫ��PA0���κδ���Ĭ�Ͼ���WKUP����
			PWR_EnterSTANDBYMode(); //�������ģʽ�����滽�Ѻ󣬳������´������ļ���ʼִ�У��൱�ڲ��ϵ�һ��
		}
		else
		{
			LED_R(ON);
			printf("KEY2����ʱ�䲻��������Ҫ�������ܽ������ģʽ\r\n");
		}
	}
	
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}
