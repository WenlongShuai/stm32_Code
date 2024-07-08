#include "stm32f10x.h"
#include "bsp_i2c_eeprom.h"
#include "bsp_led.h"
#include "bsp_usart.h"

#define ARRAYSIZE   			 10
#define EEP_Firstpage      0x00

uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];

uint8_t I2C_Test(void);

int main()
{
//	uint8_t read, i = 0;
//	uint8_t readArray[ARRAYSIZE] = {0};
//	uint8_t writeArray[ARRAYSIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9 ,10};
	
	LED_GPIO_Config();
	I2C_EEPROM_Init();
	USART_GPIO_Init();
	
	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");
	
	printf("\r\n ����һ��I2C����(AT24C02)��д�������� \r\n");	
	
	//EEPROM ��д����
	if(I2C_Test() ==1)
	{
			LED_G(ON);
	}
	else
	{
			LED_R(ON);
	}
	
	#if 0
		I2C_EEPROM_WriteByte(0x01, 0x12);
		EEPROM_WaitForWriteEnd();
		//read = I2C_EEPROM_ReadCurrent();
		read = I2C_EEPROM_ReadRandom(0x01);
		//Usart_SendByteData(USARTx, read);
		if(read == 0x12)
		{
			LED_G(ON);
		}
		else
		{
			LED_R(ON);
		}
	#elif 0
		//I2C_EEPROM_WritePage(0x00, writeArray, EEPROM_BYTE_ALIG);
		I2C_EEPROM_WriteData(0x00, writeArray, ARRAYSIZE);
		EEPROM_WaitForWriteEnd();
		I2C_EEPROM_ReadSequential(0x00, readArray, ARRAYSIZE);
		for ( i=0; i < ARRAYSIZE; i++ ) //��仺��
		{ 
			printf("0x%02X ", readArray[i]); 
		}
	#endif

	
	
	while(1)
	{
		
	}
	return 0;
}

uint8_t I2C_Test(void)
{
	uint16_t i;

	printf("д�������\n\r");
    
	for ( i=0; i<=255; i++ ) //��仺��
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //��I2c_Buf_Write��˳�����������д��EERPOM�� 
	I2C_EEPROM_WriteData(EEP_Firstpage, I2c_Buf_Write, 256);
  EEPROM_WaitForWriteEnd();
  EEPROM_INFO("\n\rд�ɹ�\n\r");
   
   EEPROM_INFO("\n\r����������\n\r");
  //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
	I2C_EEPROM_ReadSequential(EEP_Firstpage, I2c_Buf_Read, 256); 
   
  //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			EEPROM_ERROR("0x%02X ", I2c_Buf_Read[i]);
			EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��\n\r");
			return 0;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  EEPROM_INFO("I2C(AT24C02)��д���Գɹ�\n\r");
  
  return 1;
}

