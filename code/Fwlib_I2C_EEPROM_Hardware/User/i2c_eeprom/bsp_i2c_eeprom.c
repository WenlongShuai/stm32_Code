#include "bsp_i2c_eeprom.h"

/**
  * @brief  I2C ��ʱ������
  * @param  errorCode:���������
  * @retval 0
  */
static  uint8_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}

/**
  * @brief  I2C GPIO�����ü���ʼ��
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//��PB6��PB7��ʱ��
	RCC_APB2PeriphClockCmd(I2C_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C_GPIO_SCL_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = I2C_GPIO_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO_SDA_PORT, &GPIO_InitStruct);
}

/**
  * @brief  I2C ������ص����ü���ʼ��
  * @param  ��
  * @retval ��
  */
static void I2C_Config()
{
	I2C_InitTypeDef I2C_InitStruct;
	//��I2C�����ʱ��
	RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
	
	I2C_InitStruct.I2C_OwnAddress1 = I2C_OWNADDRESS1;
	I2C_InitStruct.I2C_Mode = I2C_MODE;
	I2C_InitStruct.I2C_DutyCycle = I2C_DUTYCYCLE;
	I2C_InitStruct.I2C_ClockSpeed = I2C_CLOCKSPEED;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_ACKNOWLEDGEDADDRESS;
	I2C_InitStruct.I2C_Ack = I2C_ACK;
	
	I2C_Init(DEBUG_I2C, &I2C_InitStruct);
	
	I2C_Cmd(DEBUG_I2C, ENABLE);
}

/**
  * @brief  I2C ���еĳ�ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_EEPROM_Init()
{
	I2C_GPIO_Config();
	I2C_Config();
}

/**
  * @brief  ͨ��I2C�Ѵ洢����һ���ֽ�����д�뵽EEPROM��
	* @param  writeAddr:д��EEPROM�����ݵ�ַ
						data:д��EEPROM������
  * @retval ��
  */
void I2C_EEPROM_WriteByte(uint8_t writeAddr, uint8_t data)
{
	uint32_t I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET );  //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR)  //���EV5�¼�(��Ҫ���start�ź��Ƿ������)
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(0);
			return;
		}
	} 
	
	I2C_SendData(DEBUG_I2C, 0xA0);  //�������ʹӻ��豸��ַ+д����
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ����豸��ַ+дλ�Ƿ��ͳɹ����Լ����ݼĴ����Ƿ�Ϊ�� */
//	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
//	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
//	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR)  //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(1);
			return;
		}
	}
	
	I2C_SendData(DEBUG_I2C, writeAddr);    //������ӻ�����д�����ݵĵ�ַ
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);  //������ݼĴ����Ƿ�Ϊ��
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR)  //���EV8�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(2);
			return;
		}
	}
	
	I2C_SendData(DEBUG_I2C, data);  //������ӻ�����д�������
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ������ݼĴ����Ƿ�Ϊ�գ�����ֽڷ����Ƿ���� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_BTF) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) //���EV8_2�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(3);
			return;
		}
	}
	
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
}

/**
  * @brief  I2C�Ѵ洢��������д��EEPROM�У���ҳд�룩��һҳ�����д8byte
	* @param  writeAddr:ҳд������ʱEEPROM���׵�ַ
						data:д��EEPROM������
						dataLen:д��EEPROM���ݵĴ�С
  * @retval ��
  */
void I2C_EEPROM_WritePage(uint8_t writeAddr, uint8_t *dataArray, uint8_t dataLen)
{
	uint8_t i = 0;
	uint32_t I2CTimeOut = I2Cx_LONG_TIMEOUT;
	if(dataLen > EEPROM_BYTE_ALIG)  //AT24C02��ҳд������ֽ���Ϊ8byte
	{
		return;
	} 
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) //���EV5�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(4);
			return;
		}
	}

	I2C_SendData(DEBUG_I2C, 0xA0);  //�������ʹӻ��豸��ַ+д����
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	/* ����豸��ַ+дλ�Ƿ��ͳɹ����Լ����ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR)  //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(5);
			return;
		}
	}
	
		
	I2C_SendData(DEBUG_I2C, writeAddr);    //������ӻ�����д�����ݵĵ�ַ
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);   //������ݼĴ����Ƿ�Ϊ��
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR)  //���EV8�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(6);
			return;
		}
	}

	while(dataLen--)
	{
		I2C_SendData(DEBUG_I2C, dataArray[i]);  //������ӻ�����д�������
		I2CTimeOut = I2Cx_LONG_TIMEOUT;
		/* ������ݼĴ����Ƿ�Ϊ�գ�����ֽڷ����Ƿ���� */
		//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
		//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_BTF) == RESET);
		while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR) //���EV8�¼�
		{
			if((I2CTimeOut--) == 0)
			{
				I2C_TIMEOUT_UserCallback(7);
				return;
			}
		}
		
		i++;
	}
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
}

/**
  * @brief  I2C�Ѵ洢��������д��EEPROM�У�EEPROM����ܴ洢256byte������
	* @param  writeAddr:ҳд������ʱEEPROM���׵�ַ
						data:д��EEPROM������
						dataLen:д��EEPROM���ݵĴ�С
  * @retval ��
  */
void I2C_EEPROM_WriteData(uint8_t writeAddr, uint8_t *data, uint16_t dataLen)
{
	uint8_t addr = writeAddr % EEPROM_BYTE_ALIG;   //�ж�д��ĵ�ַ�Ƿ�Ϊ��ַ����
	uint8_t writePageNum = dataLen / EEPROM_BYTE_ALIG;  //���dataLen�ж��ٸ�Page
	uint8_t writeSingleNum = dataLen % EEPROM_BYTE_ALIG;  //���dataLenд��Page֮�󣬻�ʣ���ٸ��������ֽ�����(�ղ���һҳ)
	
	uint8_t i = 0;
	
	if(addr == 0)  //д��ĵ�ַ�ֽڶ���
	{
		for(i = 0;i < writePageNum;i++)
		{
			I2C_EEPROM_WritePage(writeAddr+i*EEPROM_BYTE_ALIG, data+i*EEPROM_BYTE_ALIG, EEPROM_BYTE_ALIG);  //ÿ��д��һҳ������
			EEPROM_WaitForWriteEnd();  //д��֮�󣬷�ֹEEPROM�ڲ���ʱ�ӻ��ڽ���д��������Ȼ������EEPROM�����ź�ʱ��EEPROM�ղ����źţ�����ȴ�EEPROM�ڲ�д��
		}
		I2C_EEPROM_WritePage(writeAddr+i*EEPROM_BYTE_ALIG, data+i*EEPROM_BYTE_ALIG, writeSingleNum); //д��ʣ�µ��ֽ�����
		EEPROM_WaitForWriteEnd(); //д��֮�󣬷�ֹEEPROM�ڲ���ʱ�ӻ��ڽ���д��������Ȼ������EEPROM�����ź�ʱ��EEPROM�ղ����źţ�����ȴ�EEPROM�ڲ�д��
	}
	else    //д��ĵ�ַ�ֽ�û����
	{
		uint8_t writeAddrPagePos = writeAddr / 8;  //���writeAddr��EEPROM�е���һ��page
		uint8_t writeAddrEmptyPosNum = EEPROM_BYTE_ALIG * (1 + writeAddrPagePos) - writeAddr; //���writeAddr���ڵ�page�л�ʣ���ٸ��ֽ�
		I2C_EEPROM_WritePage(writeAddr, data, writeAddrEmptyPosNum);  //��writeAddr����ҳʣ��Ŀռ�д��
		EEPROM_WaitForWriteEnd(); //д��֮�󣬷�ֹEEPROM�ڲ���ʱ�ӻ��ڽ���д��������Ȼ������EEPROM�����ź�ʱ��EEPROM�ղ����źţ�����ȴ�EEPROM�ڲ�д��
		
		writePageNum = (dataLen - writeAddrEmptyPosNum) / EEPROM_BYTE_ALIG;  //���д��writeAddr����ҳʣ��Ŀռ�֮��ʣ������ݿ���д������Page
		for(i = 0;i < writePageNum;i++)
		{
			I2C_EEPROM_WritePage(writeAddr+writeAddrEmptyPosNum+i*EEPROM_BYTE_ALIG, data+writeAddrEmptyPosNum+i*EEPROM_BYTE_ALIG, EEPROM_BYTE_ALIG);//ÿ��д��һҳ������
			EEPROM_WaitForWriteEnd();//д��֮�󣬷�ֹEEPROM�ڲ���ʱ�ӻ��ڽ���д��������Ȼ������EEPROM�����ź�ʱ��EEPROM�ղ����źţ�����ȴ�EEPROM�ڲ�д��
		}
		
		writeSingleNum = dataLen - writePageNum * EEPROM_BYTE_ALIG - writeAddrEmptyPosNum;  //������ʣ�¶�������ûд
		I2C_EEPROM_WritePage(writeAddr+writeAddrEmptyPosNum+i*EEPROM_BYTE_ALIG, data+writeAddrEmptyPosNum+i*EEPROM_BYTE_ALIG, writeSingleNum);  //д����󼸸�����
		EEPROM_WaitForWriteEnd();//д��֮�󣬷�ֹEEPROM�ڲ���ʱ�ӻ��ڽ���д��������Ȼ������EEPROM�����ź�ʱ��EEPROM�ղ����źţ�����ȴ�EEPROM�ڲ�д��
	}
}


/**
  * @brief  I2C��ȡEEPROM��ǰ��ַ����ַ������+1���е����ݣ�1byte��
	* @param  ��
  * @retval ���ض�ȡ������
  */
uint8_t I2C_EEPROM_ReadCurrent()
{
	uint32_t I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	uint8_t readData = 0;
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR)  //���EV5�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(8);
		}
	}

	I2C_SendData(DEBUG_I2C, 0xA1);  //�������ʹӻ��豸��ַ+������
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ����豸��ַ+��λ�Ƿ��ͳɹ����Լ����ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR)  //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(9);
		}
	}
	
	
	I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //����������Ӧ���ź�
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ��ⷢ��/���ռĴ����Ƿ���յ����ݣ�������ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == SET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_RXNE) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) //���EV7�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(10);
		}
	}
	
	readData = I2C_ReceiveData(DEBUG_I2C);
	
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
	
	I2C_AcknowledgeConfig(DEBUG_I2C, ENABLE);  //��������Ӧ���ź�

	return readData;
}

/**
  * @brief  I2C�����ȡEEPROMָ����ַ�е����ݣ�1byte��
	* @param  readAddr:��ȡEEPROM���ĸ���ַ������
  * @retval ����ָ����ַ������
  */
uint8_t I2C_EEPROM_ReadRandom(uint8_t readAddr)
{
	uint8_t readData = 0;
	uint32_t I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR)  //���EV5�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(11);
		}
	}

	I2C_SendData(DEBUG_I2C, 0xA0);  //�������ʹӻ��豸��ַ+д����
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ����豸��ַ+��λ�Ƿ��ͳɹ����Լ����ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR)  //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(12);
		}
	}

	
	I2C_SendData(DEBUG_I2C, readAddr); //����������Ҫ�� �ӻ� �ĸ���ַ��ȡ����
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);   //������ݼĴ����Ƿ�Ϊ��
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR) //���EV8�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(13);
		}
	}
	
	
	/* �ڶ�����ʱ���ڶ��η�����ʼ�ź� */
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(14);
		}
	}

	I2C_SendData(DEBUG_I2C, 0xA1);  //�������ʹӻ��豸��ַ+������
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET); //�ڶ�����ʱ��ֻ��Ҫ����ַ�Ƿ񱻷���
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR)  //�����շ����EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(15);
		}
	}

	I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //����������Ӧ���ź�
	I2CTimeOut = I2Cx_FLAG_TIMEOUT;
	/* ��ⷢ��/���ռĴ����Ƿ���յ����ݣ�������ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == SET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_RXNE) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) //���EV7�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			return I2C_TIMEOUT_UserCallback(16);
		}
	}
	
	readData = I2C_ReceiveData(DEBUG_I2C);
	
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
	I2C_AcknowledgeConfig(DEBUG_I2C, ENABLE);
	
	return readData;
}

/**
  * @brief  I2C˳���ȡEEPROM������
	* @param  readAddr:��EEPROM���ĸ���ַ��ʼ��ȡ����
						readData:���ݴ�ŵĴ洢����ַ
						readLen:˳���ȡ���ݵĴ�С
  * @retval ��
  */
void I2C_EEPROM_ReadSequential(uint8_t readAddr,uint8_t *readData , uint16_t readLen)
{
	uint8_t i = 0;
	uint32_t I2CTimeOut = I2Cx_LONG_TIMEOUT;
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) //���EV5�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(17);
			return;
		}
	}

	
	I2C_SendData(DEBUG_I2C, 0xA0);  //�������ʹӻ��豸��ַ+д����
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	/* ����豸��ַ+��λ�Ƿ��ͳɹ����Լ����ݼĴ����Ƿ�Ϊ�� */
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == RESET);
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(18);
			return;
		}
	}

	
	I2C_SendData(DEBUG_I2C, readAddr); //����������Ҫ�� �ӻ� �ĸ���ַ��ȡ����
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == RESET);  //������ݼĴ����Ƿ�Ϊ��
 	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR)  //���EV8�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(19);
			return;
		}
	}


	/* �ڶ�����ʱ���ڶ��η�����ʼ�ź� */
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //���start�ź��Ƿ��ͳɹ�
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) //���EV5�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(20);
			return;
		}
	}


	I2C_SendData(DEBUG_I2C, 0xA1);  //�������ʹӻ��豸��ַ+������
	I2CTimeOut = I2Cx_LONG_TIMEOUT;
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);  //�ڶ�����ʱ��ֻ��Ҫ����ַ�Ƿ񱻷���
	while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR)  //���EV6�¼�
	{
		if((I2CTimeOut--) == 0)
		{
			I2C_TIMEOUT_UserCallback(21);
			return;
		}
	}

	
	while(readLen)
	{
		if(readLen == 1)
		{
			I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //������ӻ�����Ӧ���ź�
		}
		I2CTimeOut = I2Cx_LONG_TIMEOUT;
		/* ��ⷢ��/���ռĴ����Ƿ���յ����ݣ�������ݼĴ����Ƿ�Ϊ�� */
		//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TRA) == SET);
		//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_RXNE) == RESET);
		while(I2C_CheckEvent(DEBUG_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR)  //���EV7�¼�
		{
			if((I2CTimeOut--) == 0)
			{
				I2C_TIMEOUT_UserCallback(22);
				return;
			}
		}

		readData[i] = I2C_ReceiveData(DEBUG_I2C);
		
		i++;
		readLen--;
	}

	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
	I2C_AcknowledgeConfig(DEBUG_I2C, ENABLE);  //������ӻ�����Ӧ���ź�
}

/**
  * @brief  I2C�ڽ��ж�д������󣬵ȴ�EEPROM�ڲ�ʱ���д���
	* @param  ��
  * @retval ��
  */
void EEPROM_WaitForWriteEnd()
{	
	uint32_t I2CTimeOut1 = I2C_FLAG_TIMEOUT;
	uint32_t I2CTimeOut2 = I2C_FLAG_TIMEOUT;
	do
	{
		I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //��������start�ź�
		I2CTimeOut1 = I2C_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET) //���EV5�¼�
		{
			if((I2CTimeOut1--) == 0)
			{
				I2C_TIMEOUT_UserCallback(23);
				return;
			}
		}
		I2C_SendData(DEBUG_I2C, 0xA0);  //�������ʹӻ��豸��ַ+д����
		
		if((I2CTimeOut2--) == 0)
		{
			I2C_TIMEOUT_UserCallback(24);
			return;
		}
	}while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //��������stop�ź�
}

