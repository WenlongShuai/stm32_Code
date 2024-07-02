#include "bsp_SystemClock.h"

//ʹ�üĴ���ʵ��ϵͳʱ������
//HSE��Ϊϵͳʱ��Դ
void HSE_SetSysClock_REG(uint32_t RCC_CFGR_PLLMULLx)
{
	uint32_t HSEStatus = 0, HSEStartUpCount = 0;
	
	RCC_DeInit();  //��λRCC���мĴ���
	
	RCC->CR |= RCC_CR_HSEON;  //ʹ��HSEʱ��
	
	//�ȴ�HSEʱ�Ӿ��������������ʱ���򲻼����ж��Ƿ��������ֱ���˳�
	do
	{
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		HSEStartUpCount++;
	}while((HSEStatus != RCC_CR_HSERDY) && (HSEStartUpCount != HSE_STARTUP_TIMEOUT));
	
	//�ٴ��ж�HSEʱ���Ƿ�׼������
	if((RCC->CR & RCC_CR_HSERDY) != 0)
	{
		//������ɣ���HSEʱ�ӵ�״̬������1
		HSEStatus = (uint32_t)0x01;
	}
	else
	{
		//����δ��ɣ���HSEʱ�ӵ�״̬������0
		HSEStatus = (uint32_t)0x00;
	}
	
	//HSEʱ�Ӿ������
	if(HSEStatus == 0x01)
	{
		//FLASH ����Ԥȡ������
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    //��FLASH ��ʱ������Ϊ2���ȴ�״̬
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;  
		
		//ѡ��PLLʱ������ԴHSE�ǲ���Ƶ��
		RCC->CFGR &= RCC_CFGR_PLLXTPRE_HSE;
		
		//ѡ��PLLʱ������Դ��HSE
		RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;
		
		//����PLLʱ��Դ����ı�Ƶ
		RCC->CFGR |= RCC_CFGR_PLLMULLx;
		
		//ʹ��PLL
		RCC->CR |= RCC_CR_PLLON;
		
		//�ȴ�PLLʹ�����
		while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
		
		//ѡ��SYSCLK��ʱ��Դ����HSI��PLLCLK��HSE����ѡ��
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		
		//�ȴ�SYSCLKʱ��ѡ�����
		while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
		
		//HCLK = SYSCLK������AHB��Ƶ���ķ�Ƶϵ����HCLK=SYSCLK/1
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
		
		//PCLK1 = HCLK������APB1��Ƶ���ķ�Ƶϵ����APB1=SYSCLK/2
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
		
		//PCLK2 = HCLK,����APB2��Ƶ���ķ�Ƶϵ����APB2=SYSCLK/1
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	}
	//HSEʱ�Ӿ���δ���
	else
	{
		//HSEʱ������ʧ�ܵ�ʱ�򣬴������Ĵ���
	}
}


//HSI����2��Ƶ��Ϊϵͳʱ��Դ
void HSI_SetSysClock_REG(uint32_t RCC_CFGR_PLLMULLx)
{
	uint32_t HSIStatus = 0, HSIStartUpCount = 0;
	
	RCC_DeInit();  //��λRCC���мĴ���
	
	RCC->CR |= RCC_CR_HSION;  //ʹ��HSIʱ��
	
	//�ȴ�HSIʱ�Ӿ��������������ʱ���򲻼����ж��Ƿ��������ֱ���˳�
	do
	{
		HSIStatus = RCC->CR & RCC_CR_HSIRDY;
		HSIStartUpCount++;
	}while((HSIStatus != RCC_CR_HSIRDY) && (HSIStartUpCount != (uint16_t)0x0500));
	
	//�ٴ��ж�HSIʱ���Ƿ�׼������
	if((RCC->CR & RCC_CR_HSIRDY) != 0)
	{
		//������ɣ���HSIʱ�ӵ�״̬������1
		HSIStatus = (uint32_t)0x01;
	}
	else
	{
		//����δ��ɣ���HSIʱ�ӵ�״̬������0
		HSIStatus = (uint32_t)0x00;
	}
	
	//HSIʱ�Ӿ������
	if(HSIStatus == 0x01)
	{
		//FLASH ����Ԥȡ������
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    //��FLASH ��ʱ������Ϊ2���ȴ�״̬
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;  
		
		//ѡ��PLLʱ������Դ��HSI����2��Ƶ
		RCC->CFGR &= RCC_CFGR_PLLSRC_HSI_Div2;
		
		//����PLLʱ��Դ����ı�Ƶ
		RCC->CFGR |= RCC_CFGR_PLLMULLx;
		
		//ʹ��PLL
		RCC->CR |= RCC_CR_PLLON;
		
		//�ȴ�PLLʹ�����
		while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
		
		//ѡ��SYSCLK��ʱ��Դ����HSI��PLLCLK��HSE����ѡ��
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		
		//�ȴ�SYSCLKʱ��ѡ�����
		while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
		
		//HCLK = SYSCLK������AHB��Ƶ���ķ�Ƶϵ����HCLK=SYSCLK/1
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
		
		//PCLK1 = HCLK������APB1��Ƶ���ķ�Ƶϵ����APB1=SYSCLK/2
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
		
		//PCLK2 = HCLK,����APB2��Ƶ���ķ�Ƶϵ����APB2=SYSCLK/1
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	}
	//HSIʱ�Ӿ���δ���
	else
	{
		//HSIʱ������ʧ�ܵ�ʱ�򣬴������Ĵ���
	}
}

//ʹ�ù̼���ʵ��ϵͳʱ�ӱ��
//HSE��Ϊϵͳʱ��Դ
void HSE_SysSetClock_FWLib(uint32_t RCC_PLLMul_x)
{
	ErrorStatus HSEStatus;
	
	RCC_DeInit();  //��λRCC���мĴ���
	
	RCC_HSEConfig(RCC_HSE_ON);  //ʹ��HSEʱ��
	
	//�ȴ�HSEʱ�Ӿ��������������ʱ���򲻼����ж��Ƿ��������ֱ���˳�
	HSEStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStatus == SUCCESS)  //HSE �������
	{
		//FLASH ����Ԥȡ������
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		//����FLASH ʱ��Ϊ2���ȴ�״̬
		FLASH_SetLatency(FLASH_Latency_2);
		
		//PLLѡ��ʱ��Դ�����б�Ƶ
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
		
		//ʹ��PLL
		RCC_PLLCmd(ENABLE);
		
		//�ȴ�PLLʹ�����
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);
		
		//ѡ��SYSCLK��ʱ��Դ����HSI��PLLCLK��HSE����ѡ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		//�ȴ�SYSCLKʱ��ѡ�����
		while(RCC_GetSYSCLKSource() != 0x08);
		
		//HCLK = SYSCLK������AHB��Ƶ���ķ�Ƶϵ����HCLK=SYSCLK/1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		//PCLK1 = HCLK������APB1��Ƶ���ķ�Ƶϵ����APB1=SYSCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		//PCLK2 = HCLK,����APB2��Ƶ���ķ�Ƶϵ����APB2=SYSCLK/1
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	}
	else  //HSE ����δ���
	{
		//HSEʱ������ʧ�ܵ�ʱ�򣬴������Ĵ���
	}
}


//HSIʱ��2��Ƶ����Ϊϵͳʱ��Դ
void HSI_SysSetClock_FWLib(uint32_t RCC_PLLMul_x)
{
	uint32_t HSIStatus = 0, HSIStartUpCount = 0;
	
	RCC_DeInit();  //��λRCC���мĴ���
	
	RCC_HSICmd(ENABLE); //ʹ��HSIʱ��
	
	//�ȴ�HSIʱ�Ӿ��������������ʱ���򲻼����ж��Ƿ��������ֱ���˳�
	do
	{
		HSIStatus = RCC->CR & RCC_CR_HSIRDY;
		HSIStartUpCount++;
	}while((HSIStatus != RCC_CR_HSIRDY) && (HSIStartUpCount != (uint16_t)0x0500));
	
	//�ٴ��ж�HSIʱ���Ƿ�׼������
	if((RCC->CR & RCC_CR_HSIRDY) != 0)
	{
		//������ɣ���HSIʱ�ӵ�״̬������1
		HSIStatus = (uint32_t)0x01;
	}
	else
	{
		//����δ��ɣ���HSIʱ�ӵ�״̬������0
		HSIStatus = (uint32_t)0x00;
	}

	if(HSIStatus == 0x01)  //HSI �������
	{
		//FLASH ����Ԥȡ������
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		//����FLASH ʱ��Ϊ2���ȴ�״̬
		FLASH_SetLatency(FLASH_Latency_2);
		
		//PLLѡ��ʱ��Դ�����б�Ƶ
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);
		
		//ʹ��PLL
		RCC_PLLCmd(ENABLE);
		
		//�ȴ�PLLʹ�����
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);
		
		//ѡ��SYSCLK��ʱ��Դ����HSI��PLLCLK��HSE����ѡ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		//�ȴ�SYSCLKʱ��ѡ�����
		while(RCC_GetSYSCLKSource() != 0x08);
		
		//HCLK = SYSCLK������AHB��Ƶ���ķ�Ƶϵ����HCLK=SYSCLK/1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		//PCLK1 = HCLK������APB1��Ƶ���ķ�Ƶϵ����APB1=SYSCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		//PCLK2 = HCLK,����APB2��Ƶ���ķ�Ƶϵ����APB2=SYSCLK/1
		RCC_PCLK2Config(RCC_HCLK_Div1);
	
	}
	else  //HSI ����δ���
	{
		//HSIʱ������ʧ�ܵ�ʱ�򣬴������Ĵ���
	}
}
