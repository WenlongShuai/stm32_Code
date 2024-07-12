#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "ff.h"

FATFS fs;
FIL fil;
BYTE work[FF_MAX_SS];
DWORD fre_sect = 0;
DWORD tot_sect = 0;
DWORD used_sect = 0;

void getDeviceInformation(FATFS *fs);
void readWriteFileTest(FIL *fil, const uint8_t *filePath);
void createDirAndRenameTest(const uint8_t* newDir, const uint8_t* oldName, const uint8_t* newName);
void getFileInformation(const uint8_t* fileName);
FRESULT scan_files (char* path);

int main()
{
	char buff[256];
	FRESULT result;
	const uint8_t *filePath = "0:qq.txt";
	LED_GPIO_Config();
	USART_GPIO_Init();
	
	printf("\r\n******** ����һ��SPI FLASH �ļ�ϵͳ���ú�������ʵ�� *******\r\n");
	
	result = f_mount(&fs, "0:", 1);
	if(result == FR_OK)
	{
		printf("���ļ�ϵͳ���سɹ������Խ��в���\r\n");
		LED_G(ON);
	}
	else
	{
		LED_R(ON);
		printf("FatFs f_mount fail,does not yet have a filesystem.\r\n");
		printf("About to format...\r\n");
		result = f_mkfs("0:", 0, work, sizeof(work));
		if(result == FR_OK)
		{
			printf("f_mkfs success\r\n");
			f_unmount("0:");
			f_mount(&fs, "0:", 1);
			LED_G(ON);
		}
		else
		{
			printf("FatFs f_mkfs fail,result:%d\r\n", result);
		}
	}

	getDeviceInformation(&fs);
	readWriteFileTest(&fil, filePath);
	#if 0
	result = f_unlink("0:shuai/");
	if(result != FR_OK)
	{
		printf("f_unlink fail,result:%d\r\n", result);
	}
	#endif
	
	result = f_stat("0:shuai/", NULL);
	if(result == FR_NO_FILE)
	{
		createDirAndRenameTest("0:shuai/", filePath, "0:shuai/wen.txt");
	}
	
	getFileInformation("0:shuai/wen.txt");
	
	printf("***************** �ļ�ɨ����� ****************\r\n");
	strcpy(buff, "0:");
  scan_files(buff);
	
	while(1)
	{
	
	}
	return 0;
}


void getDeviceInformation(FATFS *fs)
{
	FRESULT result;
	DWORD fre_clst = 0;
	
	printf("*************** �豸��Ϣ��ȡ ***************\r\n");
	
	result = f_getfree("0:", &fre_clst, &fs);
	if(result != FR_OK)
	{
		printf("f_getfree fail, result : %d\r\n",result);
		return;
	}
	tot_sect = (fs->n_fatent - 2) * fs->csize * 4;
	fre_sect = fre_clst * fs->csize * 4;
	used_sect = tot_sect - fre_sect;
	printf("���豸�ܿռ䣺%d KB\r\n",tot_sect);
	printf("��ʹ�ÿռ䣺  %d KB\r\n",used_sect);
	printf("�����ÿռ䣺  %d KB\r\n",fre_sect);
}

void readWriteFileTest(FIL *fil, const uint8_t *filePath)
{
	FRESULT result;
	int8_t f_printf_flag = 0;
	UINT br = 0;
	uint8_t fileReadBuff[200] = {0};  //�䳤���飬�����С����������һ��fileSize��ֵȷ�����䳤���鲻�ܳ�ʼ����
	printf("******** �ļ���λ�͸�ʽ��д�빦�ܲ��� ********\r\n");
	
	result = f_open(fil, (const TCHAR *)filePath, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(result != FR_OK)
	{
		printf("f_open read fail, result:%d\r\n",result);
		return;
	}
	
	result = f_read(fil, fileReadBuff, f_size(fil), &br);
	if(result != FR_OK)
	{
		printf("f_read fail, result:%d\r\n",result);
		return;
	}
	printf("��ԭ�ļ�������Ϊ��\r\n%s \r\n", fileReadBuff);
	
	//�ļ���λ
	result = f_lseek(fil, f_size(fil));
	if(result != FR_OK)
	{
		printf("f_lseek fail, result:%d\r\n",result);
		return ;
	}
	
	//�����ݸ�ʽ��д���ļ�
	f_printf_flag = f_printf(fil, "���豸�ܿռ䣺%d KB\r\n\
��ʹ�ÿռ䣺  %d KB\r\n\
�����ÿռ䣺  %d KB\r\n",tot_sect,used_sect,fre_sect);
	if(f_printf_flag < 0)
	{
		printf("f_printf fail, f_printf_flag:%d\r\n",f_printf_flag);
		return ;
	}
	
	//�ļ���λ
	f_rewind(fil);
	
	result = f_read(fil, fileReadBuff, f_size(fil), &br);
	if(result != FR_OK)
	{
		printf("f_read fail, result:%d\r\n",result);
		return;
	}
	printf("\r\n������Ӻ������Ϊ��\r\n%s \r\n", fileReadBuff);
	
	result = f_close(fil);
	if(result != FR_OK)
	{
		printf("f_close read fail, result:%d\r\n",result);
		return;
	}
}

void createDirAndRenameTest(const uint8_t* newDir, const uint8_t* oldName, const uint8_t* newName)
{
	FRESULT result;
	printf("********** Ŀ¼���������������ܲ��� **********\r\n");
	
	result = f_mkdir((const TCHAR *)newDir);
	if(result != FR_OK)
	{
		printf("f_mkdir read fail, result:%d\r\n",result);
		return;
	}
	printf("�´�����Ŀ¼·��Ϊ��%s\r\n",newDir);
	
	result = f_rename((const TCHAR *)oldName, (const TCHAR *)newName);
	if(result != FR_OK)
	{
		printf("f_rename read fail, result:%d\r\n",result);
		return;
	}
	
	printf("�� %s �ļ��ƶ���������Ϊ: %s\r\n",oldName, newName);
}

void getFileInformation(const uint8_t* fileName)
{
	FILINFO fno;
	FRESULT result;
	printf("*************** �ļ���Ϣ��ȡ���� **************\r\n");
	
	result = f_stat((const TCHAR *)fileName, &fno);
	if(result != FR_OK)
	{
		printf("f_stat read fail, result:%d\r\n",result);
		return;
	}
	
	printf("%s�ļ���Ϣ���£�\r\n", fileName);
	printf("�ļ���С��%d(byte)\r\n", fno.fsize);
	printf("ʱ�����%u-%02u-%02u, %02u:%02u\r\n",\
															(fno.fdate >> 9) + 1980,\
															fno.fdate >> 5 & 15,\
															fno.fdate & 31,\
															fno.ftime >> 11,\
															fno.ftime >> 5 & 63);
	
	printf("���ԣ�%c%c%c%c%c\r\n",(fno.fattrib & AM_DIR) ? 'D' : '-',\
														  (fno.fattrib & AM_RDO) ? 'R' : '-',\
														  (fno.fattrib & AM_HID) ? 'H' : '-',\
														  (fno.fattrib & AM_SYS) ? 'S' : '-',\
														  (fno.fattrib & AM_ARC) ? 'A' : '-');
	
}


FRESULT scan_files (char* path)        /* Start node to be scanned (***also used as work area***) */
{
	FRESULT res;
	DIR dir;
	UINT i;
	FILINFO fno;
	res = f_opendir(&dir, path);                       /* Open the directory */
	if(res == FR_OK) 
	{
		for(;;) 
		{
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
			if(res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
			if(fno.fattrib & AM_DIR) 
			{                    /* It is a directory */
				i = strlen(path);
				sprintf(&path[i], "/%s", fno.fname);
				res = scan_files(path);                    /* Enter the directory */
				if (res != FR_OK) break;
				path[i] = 0;
			} 
			else
			{                                       /* It is a file. */
				printf("%s/%s\n", path, fno.fname);
			}
		}
		f_closedir(&dir);
	}
	return res;
}



