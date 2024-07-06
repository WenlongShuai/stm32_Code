#include "bsp_i2c_eeprom.h"

static void I2C_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//打开PB6、PB7的时钟
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

static void I2C_Config()
{
	I2C_InitTypeDef I2C_InitStruct;
	//打开I2C外设的时钟
	RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
	
	I2C_InitStruct.I2C_OwnAddress1 = 0x70;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	
	I2C_Init(DEBUG_I2C, &I2C_InitStruct);
	
	I2C_Cmd(DEBUG_I2C, ENABLE);
}

void I2C_EEPROM_Init()
{
	I2C_GPIO_Config();
	I2C_Config();
}


void I2C_EEPROM_WriteByte(uint8_t writeAddr, uint8_t data)
{
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	
	I2C_SendData(DEBUG_I2C, 0xA0);  //主机发送从机设备地址+写方向
	while((I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET));
	
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_SendData(DEBUG_I2C, writeAddr);    //主机向从机发送写入数据的地址
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == SET);
	
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_SendData(DEBUG_I2C, data);  //主机向从机发送写入的数据
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_TXE) == SET);
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //主机产生stop信号
}

void I2C_EEPROM_WritePage(uint8_t writeAddr, uint8_t *dataArray, uint8_t dataLen)
{
	uint8_t i = 0;
	if(dataLen > 8)
	{
		return;
	}
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	I2C_SendData(DEBUG_I2C, 0xA0);  //主机发送从机设备地址+写方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_SendData(DEBUG_I2C, writeAddr);    //主机向从机发送写入数据的地址
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	
	while(dataLen--)
	{
		I2C_SendData(DEBUG_I2C, dataArray[i]);  //主机向从机发送写入的数据
		while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
		i++;
	}
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //主机产生stop信号
}

uint8_t I2C_EEPROM_ReadCurrent()
{
	uint8_t readData = 0;
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	I2C_SendData(DEBUG_I2C, 0xA1);  //主机发送从机设备地址+读方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	readData = I2C_ReceiveData(DEBUG_I2C);
	I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //主机产生非应答信号
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //主机产生stop信号
	return readData;
}

uint8_t I2C_EEPROM_ReadRandom(uint8_t readAddr)
{
	uint8_t readData = 0;
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	
	I2C_SendData(DEBUG_I2C, 0xA0);  //主机发送从机设备地址+写方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);

	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_SendData(DEBUG_I2C, readAddr); //主机发送需要从 从机 哪个地址读取数据
	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_RXNE) == SET);

	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	//while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	I2C_SendData(DEBUG_I2C, 0xA1);  //主机发送从机设备地址+读方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_ADDR) == RESET);

	//while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	
	readData = I2C_ReceiveData(DEBUG_I2C);
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_RXNE) == SET);
	I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //主机产生非应答信号
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //主机产生stop信号
	
	return readData;
}

void I2C_EEPROM_ReadSequential(uint8_t readAddr,uint8_t *readData , uint8_t readLen)
{
	uint8_t i = 0;
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	I2C_SendData(DEBUG_I2C, 0xA0);  //主机发送从机设备地址+写方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	I2C_SendData(DEBUG_I2C, readAddr); //主机发送需要从 从机 哪个地址读取数据
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	
	I2C_GenerateSTART(DEBUG_I2C, ENABLE);  //主机产生start信号
	while( I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_SB) == RESET ); //判断start信号是否发送成功
	I2C_SendData(DEBUG_I2C, 0xA1);  //主机发送从机设备地址+读方向
	while(I2C_GetFlagStatus(DEBUG_I2C, I2C_FLAG_AF) == SET);  //等待从机设备产生应答(0)
	
	while(readLen--)
	{
		readData[i] = I2C_ReceiveData(DEBUG_I2C);
		I2C_AcknowledgeConfig(DEBUG_I2C, ENABLE);  //主机向从机发送应答信号
		i++;
	}

	I2C_AcknowledgeConfig(DEBUG_I2C, DISABLE);  //主机产生非应答信号
	I2C_GenerateSTOP(DEBUG_I2C, ENABLE);  //主机产生stop信号
}