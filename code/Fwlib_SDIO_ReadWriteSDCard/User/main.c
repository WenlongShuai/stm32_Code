#include "stm32f10x.h"
#include "bsp_sd_test.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_usart.h"


__IO uint32_t SDCardOperation = SD_OPERATION_ERASE;

int main()
{
	/* ��ʼ��LED�� */
  LED_GPIO_Config();
	/* ��ʼ���������� */
	Key_GPIO_Config();
  /*��ʼ��USART1*/
  USART_GPIO_Init();
	/*��ʼ��SDIO*/
  SD_Init();
  
	LED_B(ON);
	printf("\r\n��ӭʹ�� STM32 ������� SDCard ���ж�д����\r\n");	
	printf("�ڿ�ʼ����SD����������ǰ��������������32G���ڵ�SD��\r\n");			
	printf("��������SD������ ���ļ�ϵͳ ��ʽ��д����ɾ��SD�����ļ�ϵͳ\r\n");		
	printf("ʵ����ͨ�����Ը�ʽ����ʹ��SD���ļ�ϵͳ�����ָ̻�SD���ļ�ϵͳ\r\n");		
	printf("\r\n ��sd���ڵ�ԭ�ļ����ɻָ���ʵ��ǰ��ر���SD���ڵ�ԭ�ļ�������\r\n");		
	printf("\r\n ����ȷ�ϣ��밴�������KEY1��������ʼSD������ʵ��....\r\n");	
  
  /* Infinite loop */
  while (1)
  {	
		/*���°�����ʼ����SD����дʵ�飬����SD��ԭ�ļ�*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{
			printf("\r\n��ʼ����SD����дʵ��\r\n");	
		  SDCardTest();
		}
	} 

	return 0;
}
