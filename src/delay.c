#include "delay.h"

void delay(volatile uint32_t time)
{
    while(time--);
}
