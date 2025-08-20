#include "i2c.h"
#include "gpiob.h"
#include "systick/systick.h"

void init_i2c(void){

    init_uart();
    init_systick();
    init_gpiob();

    // enable rcc for i2c1
    RCC_APB1ENR |= (0x1 << 21);

    // i2c1 software reset on init
    I2C1_CR1 &= ~(0x1 << 15);
    I2C1_CR1 |= (0x1 << 15);

    //* SETTING THE CLOCK *
    // match input clock frequency, APB1 which is 42 MHz by default on st nucleo f446re
    I2C1_CR2 = 0;
    I2C1_CR2 = 42;

    // setting clock control register
    // formula
    // CCR = Fapb1 / 2 * Fscl(standard mode) = 42MHz / 2 * 100kHz = 210
    I2C1_CCR = 0;
    I2C1_CCR = 210;

    // setting trise 
    // trise = (trise / tapb1) + 1 = 43
    I2C1_TRISE = 43;

    // enable acks
    I2C1_CR1 &= ~(0x1 << 10);
    I2C1_CR1 |= 0x1 << 10;

    // enable i2c1 peripheral
    I2C1_CR1 &= ~(0x1);
    I2C1_CR1 |= 0x1;
}