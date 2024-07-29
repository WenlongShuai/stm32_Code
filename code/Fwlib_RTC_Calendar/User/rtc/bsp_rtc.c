#include "bsp_rtc.h"

const uint8_t *week[7] = {"��", "һ", "��", "��", "��", "��", "��"};

/*
����֮ǰ�Ĳ�����
1��ϵͳ��λ�󣬶Ժ󱸼Ĵ�����RTC�ķ��ʱ���ֹ������Ϊ�˷�ֹ�Ժ�����(BKP)������д��
����ִ�����²�����ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʣ�
	a�����üĴ���RCC_APB1ENR��PWREN��BKPENλ��ʹ�ܵ�Դ�ͺ󱸽ӿ�ʱ��
	b�����üĴ���PWR_CR��DBPλ��ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʡ�
2����Ϊ��صĿɶ��Ĵ���ֻ��RTC��APB1ʱ�ӽ�������ͬ����RTCʱ�ӵ������ر����£�
��ˣ����ڶ�ȡRTC�Ĵ���ʱ��RTC��APB1�ӿ��������ڽ�ֹ״̬����������ȱ���ȴ�
RTC_CRL�Ĵ����е�RSFλ(�Ĵ���ͬ����־)��Ӳ���á�1����

RTC���ù��̣�
1. ��ѯRTOFFλ��ֱ��RTOFF��ֵ��Ϊ��1��
2. ��CNFֵΪ1����������ģʽ
3. ��һ������RTC�Ĵ�������д����
4. ���CNF��־λ���˳�����ģʽ
5. ��ѯRTOFF��ֱ��RTOFFλ��Ϊ��1����ȷ��д�����Ѿ����
ע���������Զ���Ĵ�������д����֮���˳�����ģʽ���ȴ�д������ɵ�ʱ��ͻῨ��������ÿдһ���Ĵ�����Ҫ�ȴ�д�����
*/
static void RTC_Config(void)
{
	//��PWREN��BKPEN��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
	
	//ʹ�ܶԺ󱸼Ĵ�����RTC�ķ���
	PWR_BackupAccessCmd(ENABLE);
	
	//BKP�����������λ
	BKP_DeInit();
	
	#if defined(RTC_CLOCK_SOURCE_LSI)
	
	//ʹ���ڲ�����ʱ��LSI 
	RCC_LSICmd(ENABLE);
	//�ȴ�LSI׼������
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	//����RTCʱ��(RTCCLK)
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	//������ر�RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	//�ȴ�RTC��APB1ʱ��ͬ��������RTC��صļĴ�����ֵ�ű����£������������Ϊ0
	RTC_WaitForSynchro();
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	//����RTC��Ƶ����ʹTR_CLK=40KHz/40000-1+1=1Hz��fTR_CLK = fRTCCLK/(PRL[19:0]+1)
	RTC_SetPrescaler(40000-1);
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	//�������ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	
	#else
	//�ⲿ��������ʹ��
	RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ�LSE׼������
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//����RTCʱ��(RTCCLK)
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//������ر�RTCʱ��
	RCC_RTCCLKCmd(ENABLE);
	//�ȴ�RTC��APB1ʱ��ͬ��������RTC��صļĴ�����ֵ�ű����£������������Ϊ0
	RTC_WaitForSynchro();
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	//����RTC��Ƶ����ʹTR_CLK=32.768KHz/32768-1+1=1Hz��fTR_CLK = fRTCCLK/(PRL[19:0]+1)
	RTC_SetPrescaler(RTC_PRE);
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	//�������ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
	#endif
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_Init(&NVIC_InitStruct);
}

//�����������е�UNIXʱ���
void time_Adjust(struct rtc_time *rtc_time)
{
	uint32_t UNIX = 0;
	//RTC�ĳ�ʼ������
	RTC_Config();
	
	UNIX = mktimev(rtc_time);
	
	//UNIX-TIME_ZOOM ������ʱ���ת��UNIXʱ��д�������
	RTC_SetCounter(UNIX-TIME_ZOOM);
	
	//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
	RTC_WaitForLastTask();
}

//���RTC�Ƿ�Ϊ������λ����λ�������������¼���λ��������RTC
void RTC_CheckAndConfig(struct rtc_time *rtc_time)
{
	//˵��֮ǰû�ж�RTC����д��ʱ������������Ѿ�����λ������Ҫ��������
	if(BKP_ReadBackupRegister(RTC_BKP_DRx) != RTC_BKP_DATA)
	{
		printf("RTCû�б����ù������Ѿ���������λ\r\n");
		time_Adjust(rtc_time);
		BKP_WriteBackupRegister(RTC_BKP_DRx, RTC_BKP_DATA);
	}
	else //˵��RTC�ļ���������ʱ�������ô�ͽ��ж�ȡʱ������޸�ʱ��
	{
		printf("RTC �Ѿ����ù�������ֱ�Ӷ�ȡʱ������޸�ʱ��\r\n");
		if(RCC_GetFlagStatus(RCC_FLAG_PINRST) == SET)
		{
			printf("RTC ͨ��NRST���Ÿ�λ\r\n");
		}
		else
		{
			printf("RTC ͨ���ϵ�/���縴λ\r\n");
		}
		RCC_ClearFlag();
		
		//��PWREN��BKPEN��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
		
		//ʹ�ܶԺ󱸼Ĵ�����RTC�ķ���
		PWR_BackupAccessCmd(ENABLE);
		
		#if defined(RTC_CLOCK_SOURCE_LSI)
		
		//ʹ���ڲ�����ʱ��LSI 
		RCC_LSICmd(ENABLE);
		//�ȴ�LSI׼������
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		//����RTCʱ��(RTCCLK)
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//������ر�RTCʱ��
		RCC_RTCCLKCmd(ENABLE);
		//�ȴ�RTC��APB1ʱ��ͬ��������RTC��صļĴ�����ֵ�ű����£������������Ϊ0
		RTC_WaitForSynchro();
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		//����RTC��Ƶ����ʹTR_CLK=40KHz/40000-1+1=1Hz��fTR_CLK = fRTCCLK/(PRL[19:0]+1)
		RTC_SetPrescaler(40000-1);
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		//�������ж�
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		
		#else
		//�ⲿ��������ʹ��
		RCC_LSEConfig(RCC_LSE_ON);
		//�ȴ�LSE׼������
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		//����RTCʱ��(RTCCLK)
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		//������ر�RTCʱ��
		RCC_RTCCLKCmd(ENABLE);
		//�ȴ�RTC��APB1ʱ��ͬ��������RTC��صļĴ�����ֵ�ű����£������������Ϊ0
		RTC_WaitForSynchro();
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		//����RTC��Ƶ����ʹTR_CLK=32.768KHz/32768-1+1=1Hz��fTR_CLK = fRTCCLK/(PRL[19:0]+1)
		RTC_SetPrescaler(RTC_PRE);
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		//�������ж�
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//��һ�ζ�RTC�Ĵ�����д�����Ѿ����
		RTC_WaitForLastTask();
		#endif	
	}
}

//��ʾ��ǰʱ��(ũ��������)
void time_Display(uint32_t timeVal, struct rtc_time *rtc_time)
{
	uint32_t BJ_TimeVal = 0;
	uint8_t str[200] = {0};
	
	//����׼ʱ��ת�ɱ���ʱ��
	BJ_TimeVal = timeVal + TIME_ZOOM;
	
	to_tm(BJ_TimeVal, rtc_time);
	
	if(GetChinaCalendar(rtc_time->tm_year,rtc_time->tm_mon,rtc_time->tm_mday,str))
	{
		printf("����������%d%d-%d-%d\r\n",str[0],str[1],str[2],str[3]);
	}
	
	GetChinaCalendarStr(rtc_time->tm_year,rtc_time->tm_mon,rtc_time->tm_mday,str);
	printf("����ũ����%s\r\n",str);
	
	if(GetJieQiStr(rtc_time->tm_year,rtc_time->tm_mon,rtc_time->tm_mday,str))
	{
		printf("����ũ������С������05�죺%s\r\n",str); 
	}
	
	printf("��ǰʱ��Ϊ��%d-%d-%d\t (����%s)\t %d:%d:%d\r\n",rtc_time->tm_year,rtc_time->tm_mon,rtc_time->tm_mday,week[rtc_time->tm_wday],
																				rtc_time->tm_hour,rtc_time->tm_min,rtc_time->tm_sec);
}

//ͨ�����ڵ�����Ӧʱ��
//ʹ�ô��ڷ��͵�ʱ����Ҫ�ڷ��͵��ַ�����ӻس���
void Time_Regulate_Get(struct rtc_time *tm)
{
	uint32_t temp_num = 0;
	uint8_t day_max=0 ;
	printf("\r\n=========================����ʱ��==================");
		
	do 
	{
		printf("\r\n  ���������(Please Set Years),��Χ[1970~2038]�������ַ�����ӻس�:");
		scanf("%d",&temp_num);
		if(temp_num <1970 || temp_num >2038)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
					
		}
		else
		{	  
			printf("\n\r  ��ݱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_year = temp_num;
			break;
		}
	}while(1);


	do 
	{
		printf("\r\n  �������·�(Please Set Months):��Χ[1~12]�������ַ�����ӻس�:");
		scanf("%d",&temp_num);
		if(temp_num <1 || temp_num >12)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
					
		}
		else
		{	  
			printf("\n\r  �·ݱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_mon = temp_num;
			break;
		}
	}while(1);
		
	/*�����·ݼ����������*/
	switch(tm->tm_mon)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:					
				day_max = 31;
			break;
		
		case 4:
		case 6:
		case 9:
		case 11:
				day_max = 30;
			break;
		
		case 2:					
				 /*��������*/
				if((tm->tm_year%4==0) &&
					 ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
					 (tm->tm_mon>2)) 
						{
							day_max = 29;
						} else 
						{
							day_max = 28;
						}
			break;			
	}

	do 
	{				
		printf("\r\n  ����������(Please Set Months),��Χ[1~%d]�������ַ�����ӻس�:",day_max);
		scanf("%d",&temp_num);
		
		if(temp_num <1 || temp_num >day_max)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
		}
		else
		{
			printf("\n\r  ���ڱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_mday = temp_num;
			break;
		}
	}while(1);
		
	do 
	{				
		printf("\r\n  ������ʱ��(Please Set Hours),��Χ[0~23]�������ַ�����ӻس�:");
		scanf("%d",&temp_num);
		
		if( temp_num >23)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
		}
		else
		{
			printf("\n\r  ʱ�ӱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_hour = temp_num;
			break;
		}
	}while(1);

	do 
	{				
		printf("\r\n  ���������(Please Set Minutes),��Χ[0~59]�������ַ�����ӻس�:");
		scanf("%d",&temp_num);
		
		if( temp_num >59)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
		}
		else
		{
			printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_min = temp_num;
			break;
		}
	}while(1);

	do 
	{				
		printf("\r\n  ����������(Please Set Seconds),��Χ[0~59]�������ַ�����ӻس�:");
		scanf("%d",&temp_num);
		
		if( temp_num >59)
		{
			printf("\r\n �������������:%d��������Ҫ��",temp_num);
		}
		else
		{
			printf("\n\r  ���ӱ�����Ϊ: %d\n\r", temp_num);

			tm->tm_sec = temp_num;
			break;
		}
	}while(1);
}




