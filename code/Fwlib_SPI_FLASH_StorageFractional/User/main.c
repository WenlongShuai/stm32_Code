#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi_flash.h"

#define FLASH_BASE 						0x000000
#define FLASH_ERASE_ADDR   		FLASH_BASE

#define FLASH_INT_ADDR 				0x000000
#define FLASH_DOUBLE_ADDR  		0x000030
#define FLASH_STRING_ADDR  		0x000090

#define FLASH_WRITESIZE  			10
#define FLASH_READSIZE  			10

int intWriteBuffer[FLASH_WRITESIZE] = {0};
int intReadBuffer[FLASH_READSIZE] = {0};

double doubleWriteBuffer[FLASH_WRITESIZE] = {0};
double doubleReadBuffer[FLASH_READSIZE] = {0};

uint8_t stringWriteBuffer[50] = "FLASH �洢�ַ���ʵ��!";
uint8_t stringReadBuffer[50] = {0};

uint8_t bufferCompare(uint8_t *writeBuffer, uint8_t *readBuffer, uint16_t len);
void Flash_StorageFractional();
void Flash_StorageString();

int main()
{
	uint32_t flashId = 0;

	LED_GPIO_Config();
	USART_GPIO_Init();
	
	SPI_INFO("\r\n����һ�� 8MB ���� flash(W25Q64) �洢����С���Լ��ַ���ʵ��\r\n");
	SPI_GPIO_Init();	
	
	SPI_Flash_ReleasePowerDown();  //��ֹflash���ڵ���ģʽ
	flashId = SPI_Flash_Read_JEDECId();
	
	SPI_INFO("\r\nflash ID:0x%x\n", flashId);
	
	if(flashId == FLASH_ID)
	{
		SPI_INFO("\r\n��⵽���� flash(W25Q64)\r\n");
		
		SPI_Flash_SectorErase(FLASH_ERASE_ADDR);
		
		//Flash_StorageFractional();  //�洢������С��
		Flash_StorageString();   //�洢�ַ���
	}
	else
	{
		SPI_INFO("\r\n��ȡ���� 8MB ���� flash(W25Q64) ID\r\n");
	}
	
	while(1)
	{
	
	}
	return 0;
}

void Flash_StorageFractional()
{
	uint8_t intFlag = 0;
	uint8_t doubleFlag = 0;
	uint16_t i = 0;
	SPI_INFO("\r\nд�������Ϊ:\r\n");
	for(i = 0;i < FLASH_WRITESIZE;i++)
	{
		intWriteBuffer[i] = 100+i;
		doubleWriteBuffer[i] = 50.1314+i;
		printf("������%d\r\n ",intWriteBuffer[i]);
		printf("С����%lf\r\n ",doubleWriteBuffer[i]);
	}
	
	SPI_Flash_SequenceWriteData(FLASH_INT_ADDR, (void *)intWriteBuffer, sizeof(intWriteBuffer));
	SPI_Flash_SequenceWriteData(FLASH_DOUBLE_ADDR, (void *)doubleWriteBuffer, sizeof(doubleWriteBuffer));
	
	SPI_Flash_SequenceReadData(FLASH_INT_ADDR, (void *)intReadBuffer, sizeof(intReadBuffer));
	SPI_Flash_SequenceReadData(FLASH_DOUBLE_ADDR, (void *)doubleReadBuffer, sizeof(doubleReadBuffer));
	SPI_INFO("\r\n��ȡ������Ϊ:\r\n");
	for(i = 0;i < FLASH_READSIZE;i++)
	{
		printf("������%d\r\n ",intReadBuffer[i]);
		printf("С����%lf\r\n ",doubleReadBuffer[i]);
	}
	
	intFlag = bufferCompare((void *)intWriteBuffer, (void *)intReadBuffer, sizeof(intReadBuffer));
	doubleFlag = bufferCompare((void *)doubleWriteBuffer, (void *)doubleReadBuffer, sizeof(doubleReadBuffer));

	if(intFlag == 0 && doubleFlag == 0)
	{
		LED_G(ON);
		SPI_INFO("\r\n8MB ���� flash(W25Q64) �洢����С�����Գɹ�\r\n");
	}
	else
	{
		LED_R(ON);
		SPI_INFO("\r\n8MB ���� flash(W25Q64) �洢����С������ʧ��\r\n");
	}
}

void Flash_StorageString()
{
	uint8_t stringFlag = 0;
	uint16_t i = 0;
	SPI_INFO("\r\nд�������Ϊ:\r\n");
	
	printf("%s\r\n",stringWriteBuffer);
	
	SPI_Flash_SequenceWriteData(FLASH_STRING_ADDR, stringWriteBuffer, strlen(stringWriteBuffer));
	
	SPI_Flash_SequenceReadData(FLASH_STRING_ADDR, stringReadBuffer, strlen(stringWriteBuffer));

	SPI_INFO("\r\n��ȡ������Ϊ:\r\n");
	printf("%s\r\n",stringReadBuffer);
	
	stringFlag = bufferCompare(stringWriteBuffer, stringReadBuffer, strlen(stringWriteBuffer));

	if(stringFlag == 0)
	{
		LED_G(ON);
		SPI_INFO("\r\n8MB ���� flash(W25Q64) �洢�ַ������Գɹ�\r\n");
	}
	else
	{
		LED_R(ON);
		SPI_INFO("\r\n8MB ���� flash(W25Q64) �洢�ַ�������ʧ��\r\n");
	}
}

uint8_t bufferCompare(uint8_t *writeBuffer, uint8_t *readBuffer, uint16_t len)
{
	if(writeBuffer == NULL || readBuffer == NULL)
	{
		printf("writeBuffer or readBuffer is NULL\r\n");
		return 1;
	}
	
	while(len--)
	{
		if(*writeBuffer != *readBuffer)
		{
			return 1;
		}
		writeBuffer++;
		readBuffer++;
	}
	return 0;
}
