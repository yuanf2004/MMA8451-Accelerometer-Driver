#ifndef MMA8451_H
#define MMA8451_H

#include "i2c.h"
#include "mm4a8451_helpers.h"

// voltage on sa0 pin, user adjustable (0 or 1)
extern uint8_t sa0;

void init_mma8451(void);
void master_send_start(void);
void master_send_stop(void);
void master_send_ack(void);
void master_send_nack(void);
void master_wait_txe(void);
void master_wait_rxne(void);
void master_wait_btf(void);
void master_write_byte(uint8_t targetreg, uint8_t data);
void master_write_bytes(void);
uint8_t master_read_byte(uint8_t targetreg);
uint8_t master_wait_ack(void);
void set_ctrl_reg1(uint8_t data);
void set_active_mode(void);
void set_standby_mode(void);

int16_t read_accel(char axis, char mode);
void read_all_accel(int16_t* i_arr, char mode);

#endif 