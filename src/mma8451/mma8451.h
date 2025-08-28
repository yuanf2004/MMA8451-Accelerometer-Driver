#ifndef MMA8451_H
#define MMA8451_H

#include "mma8451_helpers.h"

void r_configure_ctrl_reg1(uint8_t data);
void r_configure_ctrl_reg2(uint8_t data);
void r_configure_ctrl_reg3(uint8_t data);
void r_configure_ctrl_reg4(uint8_t data);
void r_configure_ctrl_reg5(uint8_t data);
void set_active_mode_only(void);
void set_standby_mode_only(void);
int16_t read_accel(char axis, char mode);
void read_all_accel(int16_t* i_arr, char mode);

#endif 