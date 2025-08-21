#ifndef MMA8451_H
#define MMA8451_H

#include "i2c.h"

// voltage on sa0 pin, user adjustable (0 or 1)
uint8_t sa0 = 0;

void init_mma8451(void);

#endif 