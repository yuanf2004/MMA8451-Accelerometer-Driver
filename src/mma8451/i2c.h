#ifndef I2C_H
#define I2C_H

// rcc for i2c
#define RCC_APB1ENR *((uint32_t *)(0x40023800 + 0x40))

// base register
#define I2C1_CR1 *((uint32_t *)(0x40005400))
#define I2C1_CR2 *((uint32_t *)(0x40005400 + 0x04))
#define I2C1_DR *((uint32_t *)(0x40005400 + 0x10))
#define I2C1_SR1 *((uint32_t *)(0x40005400 + 0x14))
#define I2C1_SR2 *((uint32_t *)(0x40005400 + 0x18))
#define I2C1_CCR *((uint32_t *)(0x40005400 + 0x1C))
#define I2C1_TRISE *((uint32_t *)(0x40005400 + 0x20))

#include <stdint.h>
#include "gpiob.h"
#include "systick/systick.h"


void init_i2c(void);
    
#endif