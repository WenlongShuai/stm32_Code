#include "bsp_spi_flash.h"

/**
  * @brief  SPI ��ʱ������
  * @param  errorCode:���������
  * @retval 0
  */
static uint8_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  SPI_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}

/**
  * @brief  SPI GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void SPI_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��ʱ��
	SPI_GPIO_RCC_ClockCmd(SPI_GPIO_CLK, ENABLE);
	
	//SCK
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_GPIO_SCK_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_SCK_PORT, &GPIO_InitStruct);
	
	//MOSI
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_GPIO_MOSI_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_MOSI_PORT, &GPIO_InitStruct);
	
	//MISO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = SPI_GPIO_MISO_PIN;
	GPIO_Init(SPI_GPIO_MISO_PORT, &GPIO_InitStruct);
	
	//CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //CS��������ã���ʹ��Ӳ�������CS
	GPIO_InitStruct.GPIO_Pin = SPI_GPIO_CS_PIN;
	GPIO_Init(SPI_GPIO_CS_PORT, &GPIO_InitStruct);
}

/**
  * @brief  SPI �������ó�ʼ��
  * @param  ��
  * @retval ��
  */
static void SPI_Config()
{
	SPI_InitTypeDef SPI_InitStruct;
	
	SPIx_CLK_ClockCmd(SPIx_CLK, ENABLE);  //������SPIʱ��
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPIx_BAUDRATEPRE;
	SPI_InitStruct.SPI_CPHA = SPIx_CPHA;  //ż������
	SPI_InitStruct.SPI_CPOL = SPIx_CPOL;  //SCK�ߵ�ƽ����
	SPI_InitStruct.SPI_CRCPolynomial = SPIx_CRC;
	SPI_InitStruct.SPI_DataSize = SPIx_DATASIZE;
	SPI_InitStruct.SPI_Direction = SPIx_DIRECTION;
	SPI_InitStruct.SPI_FirstBit = SPIx_FIRSTBIT;
	SPI_InitStruct.SPI_Mode = SPIx_MODE;
	SPI_InitStruct.SPI_NSS = SPIx_NSS;
	
	SPI_Init(SPIx, &SPI_InitStruct);
	SPI_Cmd(SPIx, ENABLE);  //ʹ��SPI
}

/**
  * @brief  SPI�������ó�ʼ���Լ�GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_GPIO_Init()
{
	SPI_GPIO_Config();
	SPI_Config();
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  ͨ��SPI���߷������ݵ��ⲿFlash�У�ÿ�η���һ���ֽڴ�С�����ݣ������Ҫÿ�η���2�ֽڵĴ�С������
						������SPI�����SPIx_DATASIZE
	* @param  data:���͵�����
  * @retval flash���ص����ݣ���ΪSPI�������õ���ȫ˫�������������ڷ������ݵ�ʱ������Ҳ�ڽ��մ��ⲿflash���ص�����
  */
uint16_t SPI_Flash_SendData(uint16_t data)
{
	uint16_t receiveData = 0;
	uint32_t SPITimeOut = SPIx_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeOut--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(0);
		}
	}
	SPI_I2S_SendData(SPIx, data);
	SPITimeOut = SPIx_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeOut--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(1);
		}	
	}
	receiveData = SPI_I2S_ReceiveData(SPIx);
	
	return receiveData;
}

/**
  * @brief  ͨ��SPI���߽����ⲿFlash���ص�����
	* @param  ��
  * @retval flash���ص�����
  */
uint16_t SPI_Flash_ReceiveData()
{
	return SPI_Flash_SendData(DUMMY_BYTE);
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ�����Flash��дʹ��
	* @param  ��
  * @retval ��
  */
void SPI_Flash_WriteEnable()
{
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_WRITEENABLE);
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ��ȴ��ⲿFlash�����Ӧ�Ĺ�������flash���ڿ���״̬
	* @param  ��
  * @retval ��
  */
void SPI_Flash_WaitForBUSY()
{
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_READSTATUSREG1);
	while((SPI_Flash_ReceiveData() & 0x01) == SET);
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���ȡFlash��JEDECId
	* @param  ��
  * @retval ����Flash��JEDECId
  */
uint32_t SPI_Flash_Read_JEDECId()
{
	uint32_t JEDEC_Id = 0;
	SPI_FLASH_CS_LOW();
	
	SPI_Flash_SendData(FLASH_JEDECID);
	JEDEC_Id |= SPI_Flash_ReceiveData() << 16;
	JEDEC_Id |= SPI_Flash_ReceiveData() << 8;
	JEDEC_Id |= SPI_Flash_ReceiveData();
	SPI_FLASH_CS_HIGH();
	return JEDEC_Id;
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���ȡFlash��DeviceId
	* @param  ��
  * @retval ����Flash��DeviceId
  */
uint8_t SPI_Flash_Read_ABHDeviceId()
{
	uint8_t deviceId = 0;
	SPI_FLASH_CS_LOW();
	
	SPI_Flash_SendData(ABH_DEVICEID);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	deviceId = SPI_Flash_ReceiveData();
	
	SPI_FLASH_CS_HIGH();
	return deviceId;
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���ȡFlash��ManufacturerDeviceId
	* @param  ��
  * @retval ����Flash��ManufacturerDeviceId
  */
uint16_t SPI_Flash_Read_ManufacturerDeviceId()
{
	uint16_t ManufacturerDeviceId = 0;
	SPI_FLASH_CS_LOW();
	
	SPI_Flash_SendData(Manufacturer_DEVICEID);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(0x00);
	ManufacturerDeviceId |= SPI_Flash_ReceiveData() << 8;
	ManufacturerDeviceId |= SPI_Flash_ReceiveData();
	
	SPI_FLASH_CS_HIGH();
	return ManufacturerDeviceId;
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���ȡFlash��UniqueDeviceId
	* @param  ��
  * @retval ����Flash��UniqueDeviceId(64λ)
  */
uint64_t SPI_Flash_Read_UniqueDeviceId()
{
	uint64_t UniqueDeviceId = 0;
	uint8_t i = 0;
	SPI_FLASH_CS_LOW();
	
	SPI_Flash_SendData(Unique_DEVICEID);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	SPI_Flash_SendData(DUMMY_BYTE);
	
	for(i = 0;i < 8;i++)
	{
		UniqueDeviceId |= SPI_Flash_ReceiveData() << 8;
	}
	
	SPI_FLASH_CS_HIGH();
	return UniqueDeviceId;
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ����ⲿflash��״̬�Ĵ���д������
	* @param  ��
  * @retval ��
  */
void SPI_Flash_WriteREG(uint16_t REGData)
{
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForBUSY();
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_WRITESTATUSREG);
	SPI_Flash_SendData((REGData & 0xFF00)>>8);
	SPI_Flash_SendData(REGData & 0xFF);
	SPI_FLASH_CS_HIGH();
	SPI_Flash_WaitForBUSY();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���ȡreadAddr��ַ�е�һ���ֽ�����
	* @param  readAddr:flash���ڲ���ַ
  * @retval ����readAddr��ַ������
  */
uint8_t SPI_Flash_ReadByteData(uint32_t readAddr)
{
	uint8_t readData = 0;
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_READDATA);
	
	SPI_Flash_SendData((readAddr & 0xFF0000)>>16);
	SPI_Flash_SendData((readAddr & 0xFF00)>>8);
	SPI_Flash_SendData(readAddr & 0xFF);
	readData = SPI_Flash_ReceiveData();
	
	SPI_FLASH_CS_HIGH();
	
	return readData;
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ�˳���ȡreadAddr��ַ�ϳ���ΪreadLen������
	* @param  readAddr:flash���ڲ���ַ
						readData:���ݴ�ŵĵ�ַ
						readLen:��ȡ���ݵĳ���
  * @retval ��
  */
void SPI_Flash_SequenceReadData(uint32_t readAddr, uint8_t *readData, uint32_t readLen)
{
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_READDATA);
	
	SPI_Flash_SendData((readAddr & 0xFF0000)>>16);
	SPI_Flash_SendData((readAddr & 0xFF00)>>8);
	SPI_Flash_SendData(readAddr & 0xFF);
	
	while(readLen--)
	{
		*readData = SPI_Flash_ReceiveData();
		readData++;
	}
	
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���pageAddr��ַ��ҳд�볤��ΪpageLen�����ݵ�flash��
	* @param  pageAddr:flash���ڲ���ַ
						pageData:д�������
						pageLen:д�����ݵĳ���
  * @retval ��
  */
void SPI_Flash_PageWrite(uint32_t pageAddr, uint8_t *pageData, uint16_t pageLen)
{
	uint16_t len = pageLen;
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForBUSY();
	SPI_FLASH_CS_LOW();
	
	SPI_Flash_SendData(FLASH_PAGEPROGRAM);
	
	SPI_Flash_SendData((pageAddr & 0xFF0000) >> 16);
	SPI_Flash_SendData((pageAddr & 0xFF00) >> 8);
	SPI_Flash_SendData(pageAddr & 0xFF);
	
	if(pageLen > FLASH_PAGE_NUM)
	{
		len = FLASH_PAGE_NUM;
	}
	
	while(len--)
	{
		SPI_Flash_SendData(*pageData);
		pageData++;
	}

	SPI_FLASH_CS_HIGH();
	SPI_Flash_WaitForBUSY();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���pageAddr��ַ˳��д�볤��ΪwriteLen�����ݵ�flash��
	* @param  writeAddr:flash���ڲ���ַ
						writeData:д�������
						writeLen:д�����ݵĳ���
  * @retval ��
  */
void SPI_Flash_SequenceWriteData(uint32_t writeAddr, uint8_t *writeData, uint32_t writeLen)
{
	uint8_t addr = writeAddr % FLASH_PAGE_NUM;
	uint16_t count = FLASH_PAGE_NUM - writeAddr;  //д�����ݵĵ�ַ����ҳ������ʣ���ٸ���λ
	uint16_t flash_PageNum = writeLen / FLASH_PAGE_NUM;   //writeLen�������ж���ҳ
	uint16_t flash_singleNum = writeLen % FLASH_PAGE_NUM;  //writeLen�����а�ҳȥ������ʣ���ٸ�����������
	uint16_t i = 0;
	if(addr == 0)   //��ַ����
	{
		for(i = 0;i < flash_PageNum;i++)
		{
			SPI_Flash_PageWrite(writeAddr+i*FLASH_PAGE_NUM, writeData+i*FLASH_PAGE_NUM, FLASH_PAGE_NUM);
		}
		SPI_Flash_PageWrite(writeAddr+i*FLASH_PAGE_NUM, writeData+i*FLASH_PAGE_NUM, flash_singleNum);
	}
	else   //��ַδ����
	{
		if(count >= writeLen)
		{
			SPI_Flash_PageWrite(writeAddr, writeData, writeLen);
		}
		else
		{
			SPI_Flash_PageWrite(writeAddr, writeData, count);
			writeAddr += count;
			writeData += count;
			
			flash_PageNum = (writeLen - count) / FLASH_PAGE_NUM;
			flash_singleNum = (writeLen - count) % FLASH_PAGE_NUM;
			i = 0;
			for(i = 0;i < flash_PageNum;i++)
			{
				SPI_Flash_PageWrite(writeAddr+i*FLASH_PAGE_NUM, writeData+i*FLASH_PAGE_NUM, FLASH_PAGE_NUM);
			}
			
			SPI_Flash_PageWrite(writeAddr+i*FLASH_PAGE_NUM, writeData+i*FLASH_PAGE_NUM, flash_singleNum);
		}
	}
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���sectorEraseAddr��ַ���������ڴ�ռ䣬ÿ�β���4KB��С���ڴ�
	* @param  sectorEraseAddr:�����ռ�ĵ�ַ
  * @retval ��
  */
void SPI_Flash_SectorErase(uint32_t sectorEraseAddr)
{
	SPI_Flash_WriteEnable();
	
	SPI_Flash_WaitForBUSY();
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_SECTORERASE);
	SPI_Flash_SendData((sectorEraseAddr & 0xFF0000) >> 16);
	SPI_Flash_SendData((sectorEraseAddr & 0xFF00) >> 8);
	SPI_Flash_SendData(sectorEraseAddr & 0xFF);
	
	SPI_FLASH_CS_HIGH();
	SPI_Flash_WaitForBUSY();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ���blockEraseAddr��ַ������ڴ�ռ䣬ÿ�β���32KB/64KB��С���ڴ�
	* @param  blockEraseAddr:�����ռ�ĵ�ַ
						flag:0:����32KB   1:����64KB
  * @retval ��
  */
void SPI_Flash_BlockErase_32K_64K(uint8_t flag, uint32_t blockEraseAddr)
{
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForBUSY();
	SPI_FLASH_CS_LOW();
	if(flag == BLOCK32K)
		SPI_Flash_SendData(FLASH_BLOCKERASE_32K);
	else
		SPI_Flash_SendData(FLASH_BLOCKERASE_64K);

	SPI_Flash_SendData((blockEraseAddr & 0xFF0000) >> 16);
	SPI_Flash_SendData((blockEraseAddr & 0xFF00) >> 8);
	SPI_Flash_SendData(blockEraseAddr & 0xFF);
	
	SPI_FLASH_CS_HIGH();
	SPI_Flash_WaitForBUSY();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ�оƬ������ֱ�Ӳ�������flash�Ŀռ�
	* @param  ��
  * @retval ��
  */
void SPI_Flash_ChipErase()
{
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForBUSY();
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_CHIPERASE);
	SPI_FLASH_CS_HIGH();
	SPI_Flash_WaitForBUSY();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ������ⲿflashΪ�µ�ģʽ
	* @param  ��
  * @retval ��
  */
void SPI_Flash_PowerDown()
{
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_POWERDOWN);
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  ����ͨ��SPI�������ⲿflash����ָ�ȡ���ⲿflash���µ�ģʽ
	* @param  ��
  * @retval ��
  */
void SPI_Flash_ReleasePowerDown()
{
	SPI_FLASH_CS_LOW();
	SPI_Flash_SendData(FLASH_RELEASEPOWERDOWN);
	SPI_FLASH_CS_HIGH();
}
