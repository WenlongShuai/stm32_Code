#include "stm32f10x.h"
#include "bsp_rtc.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_beep.h"

/*
stm32��������������ʼ����ʱ��Ϊ2000.1.1 00:00:00  ������ 
ͨ�����ڿ��Ե���RTC�Ĵ����е�ʱ��
*/

extern uint8_t time_flag;
extern uint8_t alarm_flag;

struct rtc_time time = {0,0,0,1,1,2000,0};

int main()
{
	uint8_t alarm_count = 0;
	
	LED_GPIO_Config();
	USART_GPIO_Init();
	Key_GPIO_Config();
	BEEP_GPIO_Config();
	NVIC_Config();
	
	
	LED_B(ON);
	
	RTC_CheckAndConfig(&time);
	
	while(1)
	{
		if(time_flag == 1)
		{
			time_flag = 0;
			time_Display(RTC_GetCounter(), &time);
			if(alarm_flag == 1)
			{
				alarm_count++;
			}
			
			if(alarm_count == 5)
			{
				alarm_flag = 0;
				alarm_count = 0;
				BEEP_Cmd(DISABLE);
			}
		}
		
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			TimeRegulate_And_SetAlarmClock(1, &time);  //�����޸�ʱ�䣬�����޸ĵ�ʱ�����ݷ���time�ṹ����
			time_Adjust(&time);  //���޸ĵ�ʱ��ͨ��ת����UNIXʱ������RTC_CNT��
			BKP_WriteBackupRegister(RTC_BKP_DRx, RTC_BKP_DATA);  //RTC_BKP_DRx�Ĵ�����д��RTC_BKP_DATA���ݣ���ʾ�Ѿ���RTC�ļ������������ù�
		}
		else if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON)
		{
			TimeRegulate_And_SetAlarmClock(2, &time);  //�����������ӣ������޸ĵ�ʱ�����ݷ���time�ṹ����
			alarm_Adjust(&time);
		}
		
	}
	return 0;
}

