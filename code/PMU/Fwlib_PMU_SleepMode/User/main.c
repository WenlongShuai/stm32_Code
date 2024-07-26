#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"

/* ͨ����һ�жϻ��߻����¼���stm32����˯��ģʽ���� */

int main()
{
	LED_GPIO_Config();
	USART_GPIO_Init();
	KEY_EXTI_Config();
	
	LED_B(ON);
	/* ˯��ģʽ��ͨ����һ�жϻ��� */
//	printf("��WFI����֮ǰ\r\n");
//	__WFI();
//	printf("��WFI����֮��\r\n");
	
	/* ˯��ģʽ��ͨ�������¼����� */
	printf("��WFE����֮ǰ\r\n");
	__WFE();
	printf("��WFE����֮��\r\n");
	LED_G(ON);
	
	while(1)
	{
//		printf("��while��WFI����֮ǰ\r\n");
//		__WFI();
//		printf("��while��WFI����֮��\r\n");
		
		printf("��while��WFE����֮ǰ\r\n");
		__WFE();
		printf("��while��WFE����֮��\r\n");
	}
	return 0;
}

