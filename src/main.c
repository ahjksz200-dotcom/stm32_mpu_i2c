#include "stm32f4xx.h"
#include <stdint.h>
#include "i2c.h"
#include "delay.h"

#define MPU_ADDR 0x68

int main(void)
{
    SystemInit();

    // LED PC13
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER &= ~(3<<(13*2));
    GPIOC->MODER |=  (1<<(13*2));
    GPIOC->ODR |= (1<<13);

    I2C1_Init();

    while(1)
    {
        uint8_t id = I2C1_ReadReg(MPU_ADDR, 0x75);

        if(id == 0x70)
            GPIOC->ODR &= ~(1<<13);  // LED ON
        else
            GPIOC->ODR |= (1<<13);   // LED OFF

        delay(2000000);
    }
}
