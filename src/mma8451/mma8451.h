#ifndef MMA8451_H
#define MMA8451_H

#include "mma8451_helpers.h"

uint8_t mma8451_who_am_i(void);
void r_configure_ctrl_reg1(uint8_t data);
void r_configure_ctrl_reg2(uint8_t data);
void r_configure_ctrl_reg3(uint8_t data);
void r_configure_ctrl_reg4(uint8_t data);
void r_configure_ctrl_reg5(uint8_t data);
void r_configure_xyz_data_cfg(uint8_t data);
void configure_ctrl_reg1(
    uint8_t active,
    uint8_t f_read,
    uint8_t lnoise,
    uint8_t dr,
    uint8_t aslp_rate
);
void configure_ctrl_reg2(
    uint8_t mods,
    uint8_t slpe,
    uint8_t smods,
    uint8_t rst,
    uint8_t st
);
void configure_ctrl_reg3(
    uint8_t pp_od,
    uint8_t ipol,
    uint8_t wake_ff_mt,
    uint8_t wake_pulse,
    uint8_t wake_lndprt,
    uint8_t wake_trans,
    uint8_t fifo_gate
);
void configure_ctrl_reg4(
    uint8_t int_en_drdy,
    uint8_t int_en_ff_mt,
    uint8_t int_en_pulse,
    uint8_t int_en_lndprt,
    uint8_t int_en_trans, 
    uint8_t int_en_fifo,
    uint8_t int_en_aslp
);
void configure_ctrl_reg5(
    uint8_t int_cfg_drdy,
    uint8_t int_cfg_ff_mt,
    uint8_t int_cfg_pulse,
    uint8_t int_cfg_lndprt,
    uint8_t int_cfg_trans,
    uint8_t int_cfg_fifo,
    uint8_t int_cfg_aslp
);
void configure_xyz_data_cfg(
    uint8_t fs,
    uint8_t hpf_out
);
void software_reset(void);
void set_active_mode_only(void);
void set_standby_mode_only(void);
int16_t read_accel(char axis, char mode);
void read_all_accel(int16_t* i_arr, char mode);
float read_accel_converted(char axis, char mode, uint8_t fs);
void read_all_accel_converted(float* i_arr, char mode, uint8_t fs);

#endif 