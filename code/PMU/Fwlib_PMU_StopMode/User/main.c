#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_gpio_exti.h"
#include "bsp_SystemClock.h"

/* 
stm32����ֹͣģʽ(���˯��ģʽ)��WFI�����ֹͣģʽֻ��ͨ����һ�ⲿ�ж�(EXTI�ж�)���л���
WFE�����ֹͣģʽֻ��ͨ����һ�ⲿ�¼�(EXTI�¼�)���л���
*/

void delay(uint32_t time);

int main()
{
	RCC_ClocksTypeDef RCC_Clocks;
	LED_GPIO_Config();
	USART_GPIO_Init();
	KEY_EXTI_Config();
	
	
	LED_B(ON);
	printf("����stop mode֮ǰ\r\n");
	RCC_GetClocksFreq(&RCC_Clocks);
	printf("HCLK = %d M\r\n",RCC_Clocks.HCLK_Frequency / 1000000);  //72M
	printf("PCLK1 = %d M\r\n",RCC_Clocks.PCLK1_Frequency / 1000000);  //36M
	printf("PCLK2 = %d M\r\n",RCC_Clocks.PCLK2_Frequency / 1000000);  //72M
	printf("SYSCLK = %d M\r\n",RCC_Clocks.SYSCLK_Frequency / 1000000);  //72M
	
	//��Ҫ�ڽ���ֹͣģʽ֮ǰ��ʱһ�ᣬ�ȴ�������Ϣ�����ɣ�
	//�������ʱ����û�ȴ�������꣬stm32�ͽ���ֹͣģʽ���ر�������ʱ��
	delay(0xFFFFF);
	
	#if defined(INTERRUPT)
	//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); //����WFI����ֹͣģʽ����ѹ������������ģʽ
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //����WFI����ֹͣģʽ����ѹ�������ڵ͹���ģʽ
	#else
	//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE); //����WFE����ֹͣģʽ����ѹ������������ģʽ
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE); //����WFE����ֹͣģʽ����ѹ�������ڵ͹���ģʽ
	#endif
	RCC_GetClocksFreq(&RCC_Clocks);
	//����ֹͣģʽ��������֮��ϵͳ����ڲ�HSI��Ϊϵͳʱ�ӣ�����ĳ���ȫ����8M���ٶ�ִ�С�
	HSE_SysSetClock_FWLib();
	printf("����stop mode֮��\r\n");
	
	printf("HCLK = %d M\r\n",RCC_Clocks.HCLK_Frequency / 1000000);   //8M
	printf("PCLK1 = %d M\r\n",RCC_Clocks.PCLK1_Frequency / 1000000);  //4M
	printf("PCLK2 = %d M\r\n",RCC_Clocks.PCLK2_Frequency / 1000000);  //8M
	printf("SYSCLK = %d M\r\n",RCC_Clocks.SYSCLK_Frequency / 1000000);  //8M
	LED_R(ON);
	delay(0xFFFFF);
	
	while(1)
	{
		LED_G(ON);
		printf("��while�н���stop mode֮ǰ\r\n");
		//��Ҫ�ڽ���ֹͣģʽ֮ǰ��ʱһ�ᣬ�ȴ�������Ϣ�����ɣ�
		//�������ʱ����û�ȴ�������꣬stm32�ͽ���ֹͣģʽ���ر�������ʱ��
		delay(0xFFFFF);
		
		#if defined(INTERRUPT)
		//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); //����WFI����ֹͣģʽ����ѹ������������ģʽ
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //����WFI����ֹͣģʽ����ѹ�������ڵ͹���ģʽ
		#else
		//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE); //����WFE����ֹͣģʽ����ѹ������������ģʽ
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE); //����WFE����ֹͣģʽ����ѹ�������ڵ͹���ģʽ
		#endif
		
		HSE_SysSetClock_FWLib();
		printf("��while�н���stop mode֮��\r\n");
	}
	
	return 0;
}

void delay(uint32_t time)
{
	while(time--);
}

