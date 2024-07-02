#include "bsp_systick.h"

#if SYSTICK_REG

static void SysTick_REG_Config(uint32_t loadValue)
{
	SYSTICK_LOAD = loadValue;   //��ֵ����Ϊ72   ��ʱ1us   72000 ��ʱ1ms
	SYSTICK_VAL = 0;   //���㵱ǰ��������ֵ
	SYSTICK_CTRL = SysTick_CTRL_ENABLE |
								 SysTick_CTRL_CLKSOURCE | 
								 SysTick_CTRL_TICKINT;     //ѡ��ʱ��Դ��systick�ݼ���0֮������쳣�жϡ�ʹ��SysTick
}

void SysTick_REG_Delay_us(uint32_t us)
{
	uint32_t i = 0;
	SysTick_REG_Config(72);  //us
	for(i = 0;i < us;i++)
	{
		while(!(SYSTICK_CTRL & SysTick_CTRL_COUNTFLAG));  //�ж��Ƿ�ݼ���0������ǣ���SysTick_CTRL_COUNTFLAGΪ1������Ϊ0
	}
	SYSTICK_CTRL &= ~SysTick_CTRL_ENABLE;   //ֹͣSysTick��ʱ����
}

void SysTick_REG_Delay_ms(uint32_t ms)
{
	uint32_t i = 0;
	SysTick_REG_Config(72000);  //ms
	for(i = 0;i < ms;i++)
	{
		while(!(SYSTICK_CTRL & SysTick_CTRL_COUNTFLAG));
	}
	SYSTICK_CTRL &= ~SysTick_CTRL_ENABLE;
}

#elif SYSTICK_FWLIB
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

#elif SYSTICK_INT
uint32_t delayTime = 0;

void SysTick_INT_Delay_us(uint32_t us)
{
	delayTime = us;
	SysTick_Config(72);   //us
	while(delayTime);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_INT_Delay_ms(uint32_t ms)
{
	delayTime = ms;
	SysTick_Config(72000);  //ms
	while(delayTime);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

#endif
