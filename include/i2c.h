#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include <stdint.h>

void I2C1_Init(void);
void I2C1_WriteReg(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t I2C1_ReadReg(uint8_t addr, uint8_t reg);

#endif
