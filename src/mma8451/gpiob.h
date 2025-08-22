#ifndef GPIOB_H
#define GPIOB_H
// bit 1 needs to be enabled move it by 1

#include <stdint.h>

// reset and clock control for gpiob
#define RCC_AHB1ENR *((uint32_t *)(0x40023800+ 0x30))
// gpiob base 0x4002 0400
#define GPIOB_MODER *((uint32_t *)(0x40020400))
// open drain
#define GPIOB_OTYPER *((uint32_t *)(0x40020400 + 0x04))
// pull up
#define GPIOB_PUPDR *((uint32_t *)(0x40020400 + 0x0C))
// both are alternate function 4
#define GPIOB_AFRL *((uint32_t *)(0x40020400 + 0x20))

void init_gpiob(void);

#endif