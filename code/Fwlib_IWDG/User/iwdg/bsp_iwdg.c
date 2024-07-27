#include "bsp_iwdg.h"

void IWDG_Config(uint8_t prv, uint16_t rlv)
{
	RCC_LSICmd(ENABLE);  //���ڲ�����ʱ��(LSI)
	
	//���û���ö�IWDG_PR��IWDG_RLR�Ĵ�����д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//��ʱʱ�� Tout = (4*2^prv)/40*rlv (s)
	//Tout = 8/40*5 (s) = 1s
	//Ԥ��Ƶ�Ĵ���
	IWDG_SetPrescaler(prv);
	
	//��װ�ؼĴ���
	IWDG_SetReload(rlv);
	
	//����װ�Ĵ�����ֵ�����������
	IWDG_ReloadCounter();
	
	//����IWDG(���ö�IWDG_PR��IWDG_RLR�Ĵ�����д����)
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	//����װ�Ĵ�����ֵ����������У�ι������
	//����������ֵ����0��ʱ������ϵͳ��λ
	IWDG_ReloadCounter();
}
