#include "mma8451.h"

/*
Yuan Feng' s MMA8451 Driver API
PRIOR TO USAGE:

- Please ensure that variable "sa0" from "mma8451_helpers.c" is set to match the 
wiring configuration of your device's slave address 0 pin. 

- Please call the "init_mma8451()" function from "mma8451_helpers.c" before you 
proceed to use any functions within this API.
*/

void configure_ctrl_reg1(uint8_t data){ 
    // set the values of ctrl_reg1
    master_write_byte(0x2A, data);
}

void set_active_mode(void){
    // standby -> active
    configure_ctrl_reg1(0x1);
}

void set_standby_mode(void){
    // set the device in standby mode (can allow configs)
}

// todo: add a fast-mode for this sensor (8 bits only)
int16_t read_accel(char axis, char mode){
    uint8_t msb_addr, lsb_addr;
    // 14-bit accel data
    uint16_t accel = 0x0;
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

    // combine data together to get acceleration value
    accel |= (master_read_byte(msb_addr)) << 8;
    accel |= (master_read_byte(lsb_addr) >> 2);
    return accel;
}

// todo: add a fast-mode for this sensor (8 bits only)
void read_all_accel(int16_t* i_arr, char mode){
    // INPUT MUST BE OF SIZE 3 or MORE
    // fill input array with x, y, and z values
    uint8_t buffer[6];
    master_read_bytes(buffer, 0x01, 6);

    // fill input array
    int16_t x = (buffer[0] << 8) | (buffer[1] >> 2);
    i_arr[0] = x;
    int16_t y = (buffer[2] << 8) | (buffer[3] >> 2);
    i_arr[1] = y;
    int16_t z = (buffer[4] << 8) | (buffer[5] >> 2);
    i_arr[2] = z;
}
