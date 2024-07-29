#include "stm32f10x.h"
#include "bsp_rtc.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_date.h"
#include "bsp_key.h"

/*
stm32��������������ʼ����ʱ��Ϊ2000.1.1 00:00:00  ������ 
ͨ�����ڿ��Ե���RTC�Ĵ����е�ʱ��
*/

extern uint8_t flag;
struct rtc_time time = {0,0,0,1,1,2000,0};

int main()
{
	LED_GPIO_Config();
	USART_GPIO_Init();
	Key_GPIO_Config();
	NVIC_Config();
	
	
	LED_B(ON);
	
	RTC_CheckAndConfig(&time);
	
	while(1)
	{
		if(flag == 1)
		{
			flag = 0;
			time_Display(RTC_GetCounter(), &time);
		}
		
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			Time_Regulate_Get(&time);  //�����޸�ʱ�䣬�����޸ĵ�ʱ�����ݷ���time�ṹ����
			time_Adjust(&time);  //���޸ĵ�ʱ��ͨ��ת����UNIXʱ������RTC_CNT��
			BKP_WriteBackupRegister(RTC_BKP_DRx, RTC_BKP_DATA);  //RTC_BKP_DRx�Ĵ�����д��RTC_BKP_DATA���ݣ���ʾ�Ѿ���RTC�ļ������������ù�
		}
	}
	return 0;
}

