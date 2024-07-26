#include "bsp_SystemClock.h"


//ʹ�ù̼���ʵ��ϵͳʱ�ӱ��
//HSE��Ϊϵͳʱ��Դ
void HSE_SysSetClock_FWLib()
{
	ErrorStatus HSEStatus;
	RCC_HSEConfig(RCC_HSE_ON);  //ʹ��HSEʱ��
	
	//�ȴ�HSEʱ�Ӿ��������������ʱ���򲻼����ж��Ƿ��������ֱ���˳�
	HSEStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStatus == SUCCESS)  //HSE �������
	{
		//ʹ��PLL
		RCC_PLLCmd(ENABLE);
		
		//�ȴ�PLLʹ�����
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);
		
		//ѡ��SYSCLK��ʱ��Դ����HSI��PLLCLK��HSE����ѡ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		//�ȴ�SYSCLKʱ��ѡ�����
		while(RCC_GetSYSCLKSource() != 0x08);
	}
}

