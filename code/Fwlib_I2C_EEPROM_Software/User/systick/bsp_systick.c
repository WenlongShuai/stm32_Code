#include "bsp_systick.h"

void SysTick_FWLIB_Delay_us(uint32_t us)
{
	uint32_t i = 0;
	SysTick_Config(72);   //us
	for(i = 0;i < us;i++)
	{
		while(!(SysTick->CTRL & (1<<16)));  //�ж��Ƿ�ݼ���0������ǣ���SysTick_CTRL_COUNTFLAGΪ1������Ϊ0
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //ֹͣSysTick��ʱ����
}

void SysTick_FWLIB_Delay_ms(uint32_t ms)
{
	uint32_t i = 0;
	SysTick_Config(72000);  //ms
	for(i = 0;i < ms;i++)
	{
		while(!(SysTick->CTRL & (1<<16)));
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
