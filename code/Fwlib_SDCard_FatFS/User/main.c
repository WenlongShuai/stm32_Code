#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "ff.h"
#include <string.h>

FATFS fs;
FIL fil;
BYTE work[FF_MAX_SS];

UINT FatFs_ReadFile(FIL *fil, const uint8_t *filePath);
UINT FatFs_WriteFile(FIL *fil, const uint8_t *filePath);

int main()
{
	FRESULT result;
	const uint8_t *filePath = "1:SDCard�ļ�ϵͳǮǮǮǮǮǮ111.txt";
	LED_GPIO_Config();
	USART_GPIO_Init();
	
	printf("\r\n����һ�� SDCard �ļ�ϵͳFATFS��д�ļ�ʵ��\r\n");
	
	result = f_mount(&fs, "1:", 1);
	if(result == FR_OK)
	{
		printf("FatFs f_mount success,can read and write!\r\n");
		LED_G(ON);
	}
	else
	{
		LED_R(ON);
		printf("FatFs f_mount fail,does not yet have a filesystem.\r\n");
		printf("About to format...\r\n");
		result = f_mkfs("1:", 0, work, sizeof(work));
		if(result == FR_OK)
		{
			printf("f_mkfs success\r\n");
			f_unmount("1:");
			f_mount(&fs, "1:", 1);
		}
		else
		{
			printf("FatFs f_mkfs fail,result:%d\r\n", result);
		}
	}
	FatFs_WriteFile(&fil, filePath);
	FatFs_ReadFile(&fil, filePath);

	while(1)
	{
	
	}
	return 0;
}

UINT FatFs_WriteFile(FIL *fil, const uint8_t *filePath)
{
	FRESULT result;
	UINT bw = 0;
	uint8_t fileWriteBuff[] = "stm32F103 SDCard FATFS�ļ�ϵͳ����!";
	printf("\r\n****** ���������ļ�д�����... ******\r\n");
	
	//���ļ�����д�ķ�ʽ�򿪣�����ļ������ھʹ���
	result = f_open(fil, (const TCHAR *)filePath, FA_CREATE_ALWAYS | FA_WRITE);
	if(result != FR_OK)
	{
		printf("f_open write fail, result:%d\r\n",result);
		return (int)result;
	}
	
	result = f_write(fil, fileWriteBuff, strlen(fileWriteBuff), &bw);
	if(result != FR_OK)
	{
		printf("f_write fail, result:%d\r\n",result);
		return (int)result;
	}
	printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",bw);
  printf("�����ļ�д�������Ϊ��%s\r\n",fileWriteBuff);
	
	result = f_close(fil);
	if(result != FR_OK)
	{
		printf("f_close write fail, result:%d\r\n",result);
		return (int)result;
	}
	return bw;
}

UINT FatFs_ReadFile(FIL *fil, const uint8_t *filePath)
{
	FRESULT result;
	UINT br = 0;
	UINT fileSize = 0;
	uint8_t fileReadBuff[fileSize];  //�䳤���飬�����С����������һ��fileSize��ֵȷ�����䳤���鲻�ܳ�ʼ����
	printf("\r\n****** ���������ļ���ȡ����... ******\r\n");
	
	//���ļ����Զ��ķ�ʽ��
	result = f_open(fil, (const TCHAR *)filePath, FA_READ);
	if(result != FR_OK)
	{
		printf("f_open read fail, result:%d\r\n",result);
		return (int)result;
	}
		
	fileSize = f_size(fil);  //����д���ļ����ַ���С��ȷ����ȡ�ļ���buff��С
		
	result = f_read(fil, fileReadBuff, fileSize, &br);
	if(result != FR_OK)
	{
		printf("f_read fail, result:%d\r\n",result);
		return (int)result;
	}
	
	printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",br);
	printf("����ȡ�õ��ļ�����Ϊ��%s \r\n", fileReadBuff);
		
	result = f_close(fil);
	if(result != FR_OK)
	{
		printf("f_close read fail, result:%d\r\n",result);
		return (int)result;
	}
	
	return br;
}

