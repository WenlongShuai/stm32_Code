#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include <stdlib.h>

/* 
stm32ʹ�÷�ɢ�ļ�(sct)�����Ʊ������䵽�ĸ�ָ���Ŀռ䡣
�������ǽ��ڲ���64KSRAM��Ϊ�����֣�
��һ����ǰ20Kģ����Ϊ�ڲ���SRAM��
�ڶ����ֺ�40Kģ����Ϊ�ⲿ��SRAM��
���ǽ�ʹ�� _EXTER_ ���ε�ȫ�ֱ������䵽�ⲿSRAM��
û���� _EXTER_ ���εı������䵽�ڲ�SRAM��
_EXTER_ ֻ������ȫ�ֱ�����
*/

//ͨ�� _EXTER_ ���εı����洢������Ϊ EXTER ����
//__attribute__ �ؼ���ֻ������ȫ�ֱ������������ξֲ�����
#define _EXTER_ 	__attribute__ ((section("EXTER")))

void Delay(__IO u32 nCount); 

//��������� ģ���ⲿSRAM��
uint32_t _EXTER_ testValue  =7 ;  //RW-data
//��������� ģ���ⲿSRAM��
//����ȫ�ֱ�����ʼֵΪ0������ZI-data��������������testValue2������RW-data��
//��ʱ�������ı������ԣ��������������ֳ�ʼֵΪ0�Ĳ��Ż�ΪZI-data����
uint32_t testValue2  = 0;  


//�������鵽 ģ���ⲿSRAM��
uint8_t testGrup[100]  ={0};  //ZI-data
//�������鵽 ģ���ⲿSRAM��
uint8_t _EXTER_ testGrup2[100] ={1,2,3}; //RW-data


/*��ʵ���е�sct���ã���ʹ���ⲿ�洢��ʱ�������������ܲ�������
  ʹ��malloc�޷��õ������ĵ�ַ�����Ƽ���ʵ�ʹ���Ӧ��*/
/*��һ�������Ƽ���������ο��̳��е�˵��*/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	uint32_t inerTestValue =10;  //ZI-data
	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
    
  /* ��ʼ������ */
  USART_GPIO_Init();
  
  printf("\r\nSCT�ļ�Ӧ�á�������ʹ���ڲ�SRAM�����Ѷ������䵽ָ���Ŀռ� ʵ��\r\n");
  
	printf("\r\nʹ�á�	uint32_t inerTestValue = 10; ����䶨��ľֲ�������\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&inerTestValue,inerTestValue);
	
  printf("\r\nʹ�á�uint32_t _EXTER_ testValue  = 7 ;����䶨���ȫ�ֱ�����\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&testValue,testValue);
	
  printf("\r\nʹ�á�uint32_t testValue2  = 0 ; ����䶨���ȫ�ֱ�����\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&testValue2,testValue2);
	
	
	printf("\r\nʹ�á�uint8_t testGrup[100]  = {0};����䶨���ȫ�����飺\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d,%d,%d\r\n",(uint32_t)&testGrup,testGrup[0],testGrup[1],testGrup[2]);
	
  printf("\r\nʹ�á�uint8_t _EXTER_ testGrup2[100] = {1,2,3};����䶨���ȫ�����飺\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d��%d,%d\r\n",(uint32_t)&testGrup2,testGrup2[0],testGrup2[1],testGrup2[2]);
	
	
/*��ʵ���е�sct���ã���ʹ���ⲿ�洢��ʱ�������������ܲ�������
  ʹ��malloc�޷��õ������ĵ�ַ�����Ƽ���ʵ�ʹ���Ӧ��*/
  /*��һ�������Ƽ���������ο��̳��е�˵��*/
	
	uint32_t * pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); 
	if(pointer != NULL)
	{
		*(pointer)=1;
		*(++pointer)=2;
		*(++pointer)=3;	
		
		printf("\r\nʹ�á�	uint32_t *pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); ����̬����ı���\r\n");
		printf("\r\n�����Ĳ���Ϊ��\r\n*(pointer++)=1;\r\n*(pointer++)=2;\r\n*pointer=3;\r\n\r\n");
		printf("��������������ĵ�ַΪ��0x%x,�鿴����ֵ������\r\n",(uint32_t)pointer); 
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer)=%d, \r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\nʹ��malloc��̬���������������\r\n");	
	}
  
  LED_R(ON);
	

  while(1); 
 
	return 0;

}


void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

