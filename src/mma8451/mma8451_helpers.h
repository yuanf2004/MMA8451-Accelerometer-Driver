#ifndef MMA8451_HELPERS
#define MMA8451_HELPERS

#include <stdint.h>
#include "peripherals/i2c/i2c.h"

extern uint8_t sa0;

void init_mma8451(void);
void master_send_start(void);
void master_send_stop(void);
void master_send_ack(void);
void master_send_nack(void);
void master_check_addr_sent(void);
void master_clear_addr(void);
void master_wait_txe(void);
void master_wait_rxne(void);
void master_wait_btf(void);
void master_write_byte(uint8_t targetreg, uint8_t data);
void master_write_bytes(uint8_t targetreg, uint8_t* d_arr, uint8_t arrlen);
uint8_t master_read_byte(uint8_t targetreg);
void master_read_bytes(uint8_t *buffer, uint8_t targetreg, uint8_t rlen);

#endif