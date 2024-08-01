#include "bsp_readWriteInterFlash.h"


void write_Protect_Enable(void)
{
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_EnableWriteProtection(FLASH_WRProt_AllPages);  //��Ҫ������ҳ
	NVIC_SystemReset();  //д������ʱ����Ҫ����ϵͳ������װ��ѡ���ֽ�
}

void write_Protect_Disable(void)
{
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_EnableWriteProtection(0x00000000);  //��Ҫ���������ҳ
	NVIC_SystemReset();  //д������ʱ����Ҫ����ϵͳ������װ��ѡ���ֽ�
}


void read_Protect_Enable(void)
{
	FLASH_Unlock();
	FLASH_ReadOutProtection(ENABLE);
	/* ���ö�����ѡ���ֽ�֮����Ҫ�ϵ縴λʹѡ���ֽ���Ч */
}

void read_Protect_Disable(void)
{
	FLASH_Unlock();
	FLASH_ReadOutProtection(DISABLE);
	/* ���ö�����ѡ���ֽ�֮����Ҫ�ϵ縴λʹѡ���ֽ���Ч */
}

void WriteProtect_Toggle(void)
{
	if(FLASH_GetWriteProtectionOptionByte() !=0xFFFFFFFF)
	{
		printf("\r\nоƬ����д����״̬���������н������...\r\n");
		FLASH_Unlock();
		
		FLASH_EraseOptionBytes();
		
		FLASH_EnableWriteProtection(0x00000000);  //��Ҫ���������ҳ
		
		printf("\r\nоƬ�Զ���λ���������ã���λ��оƬ�ʹ�����д����״̬\r\n");
		
		NVIC_SystemReset();  //д������ʱ����Ҫ����ϵͳ������װ��ѡ���ֽ�
	}
	else
	{
		printf("\r\nоƬ������д����״̬����������д����...\r\n");
		
		FLASH_Unlock();
		
		FLASH_EraseOptionBytes();
		
		FLASH_EnableWriteProtection(FLASH_WRProt_AllPages);  //��Ҫ������ҳ
		
		printf("\r\nоƬ�Զ���λ���������ã���λ��оƬ�ʹ���д����״̬\r\n");
		
		NVIC_SystemReset();  //д������ʱ����Ҫ����ϵͳ������װ��ѡ���ֽ�
	}
}

void ReadProtect_Toggle(void)
{
	if(FLASH_GetReadOutProtectionStatus () == SET)
	{
		printf("оƬ���ڶ�����״̬���������н��������״̬...\r\n");
		
		FLASH_Unlock();
		
		printf("���������������������������FLASH�������������\r\n");
		printf("���ڽ���������գ����Ժ��治�����κ���ʾ���\r\n");
		printf("�ȴ�20��󼴿ɸ�оƬ�����µĳ���...\r\n");
		
		FLASH_ReadOutProtection(DISABLE);
	}
	else
	{
		printf("\r\nоƬ�����޶�����״̬���������ж�����...\r\n");
		
		FLASH_Unlock();
		
		FLASH_ReadOutProtection(ENABLE);
		
		printf("\r\nоƬ�ѱ�����Ϊ���������ϵ縴λ����Ч���������¸��������ϵ磬ֻ����λ����Ч��\r\n");
		printf("\r\n���ڱ���״̬���޷����������³��򣬱���Ҫ�Ƚ������״̬������\r\n");
	}
}

