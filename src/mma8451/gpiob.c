#include "gpiob.h"

// set up gpiob pb6 and pb7 for i2c
void init_gpiob(){
    // activate rcc for gpiob
    RCC_AHB1ENR |= 0x1 << 1;

    // set alternate function mode for pb6 and pb7
    GPIOB_MODER &= ~((0x3 << (2 * 6)) | (0x3 << (2 * 7)));
    GPIOB_MODER |= ((0x2 << (2 * 6)) | (0x2 << (2 * 7)));

    // set both pins to open drain, devices need to pull line low
    GPIOB_OTYPER |= ((0x1 << 6) | (0x1 << 7));

    // set both pins to pull up 
    GPIOB_PUPDR &= ~((0x3 << (2 * 6)) | (0x3 << (2 * 7)));
    GPIOB_PUPDR |= ((0x1 << (2 * 6)) | (0x1 << (2 * 7)));

    // set alternate function 4 for both pins for i2c activation
    GPIOB_AFRL &= ~((0xF << (4 * 6)) | (0xF << (4 * 7)));
    GPIOB_AFRL |= ((0x4 << (4 * 6)) | (0x4 << (4 * 7)));
}