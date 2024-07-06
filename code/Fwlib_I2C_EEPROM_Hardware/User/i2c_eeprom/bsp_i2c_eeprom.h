#ifndef __BSP_I2C_EEPROM_H__
#define __BSP_I2C_EEPROM_H__

#include "stm32f10x.h"


#define I2C_GPIO_CLK      RCC_APB2Periph_GPIOB
#define I2C_GPIO_SCL_PORT   GPIOB
#define I2C_GPIO_SCL_PIN  GPIO_Pin_6
#define I2C_GPIO_SDA_PORT GPIOB
#define I2C_GPIO_SDA_PIN  GPIO_Pin_7

#define DEBUG_I2C I2C1
#define I2C_CLK  RCC_APB1Periph_I2C1

void I2C_EEPROM_Init();
void I2C_EEPROM_WriteByte(uint8_t writeAddr, uint8_t data);
void I2C_EEPROM_WritePage(uint8_t writeAddr, uint8_t *dataArray, uint8_t dataLen);
uint8_t I2C_EEPROM_ReadCurrent();
uint8_t I2C_EEPROM_ReadRandom(uint8_t readAddr);
void I2C_EEPROM_ReadSequential(uint8_t readAddr,uint8_t *readData , uint8_t readLen);

#endif /* __BSP_I2C_EEPROM_H__ */
