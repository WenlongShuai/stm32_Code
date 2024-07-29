#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include "stm32f10x.h"
#include "bsp_date.h"
#include "bsp_usart.h"
#include "bsp_calendar.h"


//#define RTC_CLOCK_SOURCE_LSI
#define RTC_CLOCK_SOURCE_LSE

//����ʱ��+8 == ��������ʱ��     GMT+8 == GMT
#define TIME_ZOOM   (8*60*60)


/* �ж�RTC��ʱ���Ƿ����ù� */
#define RTC_BKP_DRx			BKP_DR1
#define RTC_BKP_DATA		0xA5A5


#define RTC_PRE       		0x7FFF  //32767
#define RTC_IRQ       		RTC_IRQn
#define RTC_IRQHANDLER  	RTC_IRQHandler


void NVIC_Config(void);
void RTC_CheckAndConfig(struct rtc_time *rtc_time);
void time_Display(uint32_t timeVal, struct rtc_time *rtc_time);
void Time_Regulate_Get(struct rtc_time *tm);
void time_Adjust(struct rtc_time *rtc_time);

#endif /* __BSP_RTC_H__ */
