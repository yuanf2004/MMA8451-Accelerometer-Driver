#ifndef MMA8451_H
#define MMA8451_H

#include "mma8451_helpers.h"

void configure_ctrl_reg1(uint8_t data);
void set_active_mode(void);
void set_standby_mode(void);
int16_t read_accel(char axis, char mode);
void read_all_accel(int16_t* i_arr, char mode);

#endif 