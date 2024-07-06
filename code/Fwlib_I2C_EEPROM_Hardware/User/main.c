#include "stm32f10x.h"
#include "bsp_i2c_eeprom.h"
#include "bsp_led.h"

int main()
{
	uint8_t read = 0;
	LED_GPIO_Config();
	I2C_EEPROM_Init();
	
	I2C_EEPROM_WriteByte(0x00, 0x12);
	//read = I2C_EEPROM_ReadCurrent();
	read = I2C_EEPROM_ReadRandom(0x00);
	if(read == 0x12)
	{
		LED_G(ON);
	}
	else
	{
		LED_R(ON);
	}
	
	while(1)
	{
		
	}
	return 0;
}

