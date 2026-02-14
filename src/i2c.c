#include "i2c.h"

void I2C1_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB6 SCL, PB7 SDA AF4
    GPIOB->MODER &= ~(0xF << (6*2));
    GPIOB->MODER |=  (0xA << (6*2));

    GPIOB->OTYPER |= (1<<6) | (1<<7);
    GPIOB->OSPEEDR |= (0xF << (6*2));
    GPIOB->PUPDR &= ~(0xF << (6*2));
    GPIOB->PUPDR |=  (0x5 << (6*2));

    GPIOB->AFR[0] |= (4<<(6*4)) | (4<<(7*4));

    I2C1->CR1 = 0;
    I2C1->CR2 = 16;      // APB1 = 16MHz
    I2C1->CCR = 80;      // 100kHz
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_WriteReg(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = addr << 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = reg;

    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;

    while(!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

uint8_t I2C1_ReadReg(uint8_t addr, uint8_t reg)
{
    uint8_t data;

    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = addr << 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = reg;

    while(!(I2C1->SR1 & I2C_SR1_BTF));

    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = (addr << 1) | 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_STOP;

    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data = I2C1->DR;

    return data;
}
