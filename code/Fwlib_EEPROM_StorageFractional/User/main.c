#include "stm32f10x.h"
#include "bsp_i2c_eeprom.h"
#include "bsp_led.h"
#include "bsp_usart.h"

#define ARRAYSIZE   			 10
#define EEP_Firstpage_int      10
#define EEP_Firstpage_double   60
#define EEP_Firstpage_string   150

int intWriteBuffer[ARRAYSIZE] = {0};
double doubleWriteBuffer[ARRAYSIZE] = {0};

int intReadBuffer[ARRAYSIZE] = {0};
double doubleReadBuffer[ARRAYSIZE] = {0};

uint8_t *stringWrite = "EEPROM�洢�ַ���ʵ��!";
uint8_t stringRead[50] = {0};

void I2C_StorageFractional(void);
uint8_t bufferCompare(uint8_t *buffer1, uint8_t *buffer2, uint8_t len);

int main()
{	
	LED_GPIO_Config();
	I2C_EEPROM_Init();
	USART_GPIO_Init();
	
//	printf("\r\n ����һ��I2C����(AT24C02)�洢������С���Ĳ������� \r\n");
//	I2C_StorageFractional();
	
	printf("\r\n ����һ��I2C����(AT24C02)�洢�ַ����Ĳ������� \r\n");
	I2C_StorageString();

	while(1)
	{
		
	}
	return 0;
}

void I2C_StorageFractional(void)
{
	uint16_t i;
	uint8_t intFlag = 0;
	uint8_t doubleFlag = 0;
	

	EEPROM_INFO("\r\nд�������\n\r");
    
	for ( i=0; i < ARRAYSIZE; i++ ) //��仺��
  {   
    intWriteBuffer[i] = 100+i;
		doubleWriteBuffer[i] = 200+i+0.123456;

    printf("����Ϊ��%d\r\n ", intWriteBuffer[i]);
    printf("С��Ϊ��%lf\r\n ", doubleWriteBuffer[i]);
   }

  //������������д��EERPOM�� 
	I2C_EEPROM_WriteData(EEP_Firstpage_int, (void*)intWriteBuffer, sizeof(intWriteBuffer));
  EEPROM_WaitForWriteEnd();
	 
	//��С��������д��EERPOM�� 
	I2C_EEPROM_WriteData(EEP_Firstpage_double, (void*)doubleWriteBuffer, sizeof(doubleWriteBuffer));
  EEPROM_WaitForWriteEnd();
	 
  EEPROM_INFO("\n\rд�ɹ�\n\r");
   
  EEPROM_INFO("\n\r����������\n\r");
	 
  //��EEPROM�������������ݱ��浽intReadBuffer��
	I2C_EEPROM_ReadSequential(EEP_Firstpage_int, intReadBuffer, sizeof(intReadBuffer)); 
	 
	//��EEPROM������С�����ݱ��浽doubleReadBuffer��
	I2C_EEPROM_ReadSequential(EEP_Firstpage_double, doubleReadBuffer, sizeof(doubleReadBuffer)); 
   
	intFlag = bufferCompare((void *)intWriteBuffer,(void *)intReadBuffer,sizeof(intWriteBuffer));
	doubleFlag = bufferCompare((void *)doubleWriteBuffer,(void *)doubleReadBuffer,sizeof(doubleWriteBuffer));
	 
	if(intFlag == 0 && doubleFlag == 0)
	{
		//��intReadBuffer��doubleReadBuffer�е�����ͨ�����ڴ�ӡ
		for (i=0; i<ARRAYSIZE; i++)
		{	
			printf("����Ϊ��%d\r\n ", intReadBuffer[i]);
			printf("С��Ϊ��%lf\r\n ", doubleReadBuffer[i]);
		}
		LED_G(ON);
		EEPROM_INFO("I2C(AT24C02)�洢����С�����Գɹ�\n\r");
	}
	else
	{
		LED_R(ON);
		EEPROM_INFO("I2C(AT24C02)�洢����С������ʧ��\n\r");
	}
}


void I2C_StorageString(void)
{
	uint16_t i;
	uint8_t strFlag = 0;
	

	EEPROM_INFO("\r\nд�������\n\r");
    
	printf("%s\n\r",stringWrite);

  //���ַ���д��EERPOM�� 
	I2C_EEPROM_WriteData(EEP_Firstpage_string, stringWrite, strlen(stringWrite));
  EEPROM_WaitForWriteEnd();
	
  EEPROM_INFO("\n\rд�ɹ�\n\r");
   
  EEPROM_INFO("\n\r����������\n\r");
	 
  //��EEPROM�������ַ������浽stringRead��
	I2C_EEPROM_ReadSequential(EEP_Firstpage_string, stringRead, strlen(stringWrite)); 
	  
	strFlag = bufferCompare(stringWrite,stringRead,strlen(stringWrite));
	 
	if(strFlag == 0)
	{
		//��stringRead�е�����ͨ�����ڴ�ӡ
		printf("%s\n\r",stringRead);
		LED_G(ON);
		EEPROM_INFO("I2C(AT24C02)�洢�ַ������Գɹ�\n\r");
	}
	else
	{
		LED_R(ON);
		EEPROM_INFO("I2C(AT24C02)�洢�ַ�������ʧ��\n\r");
	}
}


uint8_t bufferCompare(uint8_t *buffer1, uint8_t *buffer2, uint8_t len)
{
	while(len--)
	{
		if(*buffer1 != *buffer2)
		{
			return 1;
		}
		buffer1++;
		buffer2++;
	}
	
	return 0;
}

