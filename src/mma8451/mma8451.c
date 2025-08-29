#include "mma8451.h"

/*
Yuan Feng' s MMA8451 Driver API
PRIOR TO USAGE:

- Please ensure that variable "sa0" from "mma8451_helpers.c" is set to match the 
wiring configuration of your device's slave address 0 pin. 

- Please call the "init_mma8451()" function from "mma8451_helpers.c" before you 
proceed to use any functions within this API.
*/

uint8_t mma8451_who_am_i(void){
// check if sensor communication is functioning, successful return is 0x1A
    return master_read_byte(0x0D);
}

// r_configure - raw configuration of registers with byte input

void r_configure_ctrl_reg1(uint8_t data){ 
    // write to ctrl_reg1
    master_write_byte(0x2A, data);
}

void r_configure_ctrl_reg2(uint8_t data){
    // write to ctrl_reg2
    master_write_byte(0x2B, data);
}

void r_configure_ctrl_reg3(uint8_t data){
    // write to ctrl_reg3
    master_write_byte(0x2C, data);
}

void r_configure_ctrl_reg4(uint8_t data){
    // write to ctrl_reg4
    master_write_byte(0x2D, data);
}

void r_configure_ctrl_reg5(uint8_t data){
    // write to ctrl_reg5
    master_write_byte(0x2E, data);
}

void r_configure_xyz_data_cfg(uint8_t data){
    // write to xyz_data_cfg reg
    master_write_byte(0x0E, data);
}

// configure - simplified ctrl_reg configurations 

void configure_ctrl_reg1(
    uint8_t active,
    uint8_t f_read,
    uint8_t lnoise,
    uint8_t dr,
    uint8_t aslp_rate
){
    /*
    active - 0
    f_read - 1
    lnoise - 2
    dr - [5:3]
    aslp_rate [7:6] 
    */
    uint8_t config = 0x0;
    config |= active;
    config |= f_read << 1;
    config |= lnoise << 2;
    config |= dr << 3;
    config |= aslp_rate << 6;

    master_write_byte(0x2A, config);
}

void configure_ctrl_reg2(
    uint8_t mods,
    uint8_t slpe,
    uint8_t smods,
    uint8_t rst,
    uint8_t st
){
    uint8_t config = 0x0;
    config |= mods;
    config |= slpe << 2;
    config |= smods << 3;
    config |= rst << 6;
    config |= st << 7;

    master_write_byte(0x2B, config);
}

void configure_ctrl_reg3(
    uint8_t pp_od,
    uint8_t ipol,
    uint8_t wake_ff_mt,
    uint8_t wake_pulse,
    uint8_t wake_lndprt,
    uint8_t wake_trans,
    uint8_t fifo_gate
){
    uint8_t config;
    config |= pp_od;
    config |= ipol << 1;
    config |= wake_ff_mt << 3;
    config |= wake_pulse << 4;
    config |= wake_lndprt << 5;
    config |= wake_trans << 6;
    config |= fifo_gate << 7;

    master_write_byte(0x2C, config);
}

void configure_ctrl_reg4(
    uint8_t int_en_drdy,
    uint8_t int_en_ff_mt,
    uint8_t int_en_pulse,
    uint8_t int_en_lndprt,
    uint8_t int_en_trans, 
    uint8_t int_en_fifo,
    uint8_t int_en_aslp
){
    uint8_t config;
    config |= int_en_aslp;
    config |= int_en_fifo << 2;
    config |= int_en_trans << 3;
    config |= int_en_lndprt << 4;
    config |= int_en_pulse << 5;
    config |= int_en_ff_mt << 6;
    config |= int_en_drdy << 7;

    master_write_byte(0x2D, config);
}

void configure_ctrl_reg5(
    uint8_t int_cfg_drdy,
    uint8_t int_cfg_ff_mt,
    uint8_t int_cfg_pulse,
    uint8_t int_cfg_lndprt,
    uint8_t int_cfg_trans,
    uint8_t int_cfg_fifo,
    uint8_t int_cfg_aslp
){
    uint8_t config;
    config |= int_cfg_drdy;
    config |= int_cfg_ff_mt << 2;
    config |= int_cfg_pulse << 3;
    config |= int_cfg_lndprt << 4;
    config |= int_cfg_trans << 5;
    config |= int_cfg_fifo << 6;
    config |= int_cfg_aslp << 7;

    master_write_byte(0x2E, config);
}

void configure_xyz_data_cdg(
    uint8_t fs,
    uint8_t hpf_out
){
    uint8_t config;
    config |= fs;
    config |= hpf_out << 4;
    
    master_write_byte(0x0E, config);
}

//todo: need to write this
void software_reset(void){
}

void set_active_mode_only(void){
    // Only set active mode (sensing)
    r_configure_ctrl_reg1(0x1);
}

void set_standby_mode_only(void){
    // Only set standby mode (configuration)
    r_configure_ctrl_reg1(0x0);
}

int16_t read_accel(char axis, char mode){
    // regular mode (14 bits)
    uint8_t msb_addr, lsb_addr;
    // 14-bit accel data
    int16_t accel = 0x0;
    if(axis == 'x'){
        msb_addr = 0x01;
        lsb_addr = 0x02;
    }
    else if(axis == 'y'){
        msb_addr = 0x03;
        lsb_addr = 0x04;
    }
    else if(axis == 'z'){
        msb_addr = 0x05;
        lsb_addr = 0x06;
    }
    else{
        // invalid argument
        return 0;
    }

    if(mode == 'r'){
        // combine data together to get acceleration value
        accel |= master_read_byte(msb_addr) << 8;
        accel |= master_read_byte(lsb_addr);
        accel >>= 2;
        if(accel & 0x2000){
            accel |= 0xC000;
        }

        return accel;
    }
    // fast mode (8 bits)
    else if(mode == 'f'){
        accel |= (master_read_byte(msb_addr));
        if(accel & 0x80){
            accel |= 0xFF00;
        }
        return accel;
    }
    else{
        // invalid char
        return -1;
    }
}

void read_all_accel(int16_t* i_arr, char mode){
    // INPUT MUST BE OF SIZE 3 or MORE
    // fill input array with x, y, and z values

    uint8_t buffer[6];
    master_read_bytes(buffer, 0x01, 6);

    if(mode == 'r'){
        // fill input array
        int16_t xtest = buffer[0];
        int16_t x = (buffer[0] << 8 | buffer[1]) >> 2;
        if(x & 0x2000){
            x |= 0xC000;
        }
        i_arr[0] = x;
        int16_t y = (buffer[2] << 8 | buffer[3]) >> 2;
        if(y & 0x2000){
            y |= 0xC000;
        }
        i_arr[1] = y;
        int16_t z = (buffer[4] << 8 | buffer[5]) >> 2;
        if(z & 0x2000){
            z |= 0xC000;
        }
        i_arr[2] = z;
    }
    else if(mode == 'f'){ 
        int8_t x = buffer[0];
        if(x & 0x80){
            x |= 0xFF00;
        }
        i_arr[0] = x;
        int8_t y = buffer[1];
        if(y & 0x80){
            y |= 0xFF00;
        }
        i_arr[1] = y;
        int8_t z = buffer[2];
        if(z & 0x80){
            z |= 0xFF00;
        }
        i_arr[2] = z;
    }
    else{
        // invalid mode
        return;
    }
}

float read_accel_converted(char axis, char mode, uint8_t fs){
    // function requires known full scale range for proper conversion
    // handle count based on full
    float count;
    if(fs == 2) count = 4096;
    else if(fs == 4) count = 2048;
    else if(fs == 8) count = 1024;
    else return -1;

    return read_accel(axis, mode) / count * 9.81;
}

void read_all_accel_converted(float* i_arr, char mode, uint8_t fs){
    // function requires known full scale range for proper conversion
    float count;
    if(fs == 2) count = 4096;
    else if(fs == 4) count = 2048;
    else if(fs == 8) count = 1024;
    else return;
    
    int16_t xyz_buffer[3];
    read_all_accel(xyz_buffer, mode);

    // update all elements of the input array to match conversion
    for(int i = 0; i < 3; i++){
        i_arr[i] =  xyz_buffer[i] / count * 9.81;
    }
}