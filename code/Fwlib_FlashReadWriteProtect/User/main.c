#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_readWriteInterFlash.h"

/* stm32�����ڲ�FLASH��д����ʵ�� */

int main(void)
{ 	
	  /*��ʼ��USART������ģʽΪ 115200 8-N-1*/
  USART_GPIO_Init();
	LED_GPIO_Config();
	Key_GPIO_Config();

	LED_G(ON);
	
	//оƬ�Զ���λ�󣬴��ڿ�����С�����쳣����������һ����������
	printf("\r\n\n\n��ӭʹ��Ұ��  STM32  �����塣\r\n");	
	printf("���Ƕ�д��������ʵ��\r\n");
	
	/* ��ȡд�����Ĵ�����ֵ�����жϣ��Ĵ���λΪ0��ʾ�б�����Ϊ1��ʾ�ޱ��� */
	/*  ��������0xFFFFFFFF����˵���в���ҳ��д������ */
	if(FLASH_GetWriteProtectionOptionByte() !=0xFFFFFFFF )
	{
		printf("\r\nĿǰоƬ����д����״̬����Key1���������\r\n");
		printf("д�����Ĵ�����ֵ��WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	else //��д����
	{
		printf("\r\nĿǰоƬ�� д �������� Key1 �������ó� д ����\r\n");
		printf("д�����Ĵ�����ֵ��WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	
	
	/*  ������SET��˵�����ڶ�����״̬ */
	if(FLASH_GetReadOutProtectionStatus () == SET )
	{
		printf("\r\nĿǰоƬ���ڶ�����״̬����Key2���������\r\n");
	}
	else
	{
		printf("\r\nĿǰоƬ�� �� �������� Key2 �������ó� �� ����\r\n");
	}
	
	LED_G(OFF);
	while(1)                            
	{	   
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			LED_TOGGLE(R);
			WriteProtect_Toggle();
		} 

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			LED_TOGGLE(B)
			ReadProtect_Toggle();			
		}		
	}	
	return 0;
}