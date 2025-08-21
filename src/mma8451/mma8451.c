#include "mma8451.h"

// TODO: at some point, split up helper functions with actual user functions

// TODO: allow for config of ctrlreg and then set active mode with bitwise or 

void init_mma8451(void){
    // initialize the driver

    init_i2c();
}

void master_send_start(){
    I2C1_CR1 |= (0x1 << 8);
    //wait for start bit registered
    while(!(I2C1_SR1 & 0x1)){}
}

void master_send_stop(){
    I2C1_CR1 |= (0x1 << 9);
}

void master_wait_txe(void){
    // wait for txe (transmit) data register to be ready (empty)
    while(!(I2C1_SR1 & (0x1 << 7))){}
}

void master_wait_btf(void){
    // wait for byte transfer to be finished
    while(!(I2C1_SR1 & (0x1 << 2))){}
}

void master_write_byte(uint8_t targetreg, uint8_t data){
// single byte write, a for if sa0 is GND or 3.3v
    uint8_t addr;
    // sa0 = 3.3v 
    if(sa0) addr = (0x1D << 1);
    // sa0 = GND
    else addr = (0x1C << 1);

    master_send_start();
    if(master_wait_ack == -1) return;

    // send slave address
    master_wait_txe();
    I2C1_DR = addr;
    master_wait_btf();
    if(master_wait_ack == -1) return;
    
    // send target register address
    master_wait_txe();
    I2C1_DR = targetreg;
    master_wait_btf();
    if(master_wait_ack() == -1) return;

    // send data 
    master_wait_txe();
    I2C1_DR = data;
    master_wait_btf();
    if(master_wait_ack() == -1) return;

    master_send_stop();
}

void master_write_bytes(){
// multi byte write

}





int8_t master_wait_ack(void){
    // wait for address sent confirmation
    while(!(I2C1_SR1 & (0x1 << 1))){}
    // check for nack, bit is set 1 if nack
    if(I2C1_SR1 & (0x1 << 10)){
        return -1;
    }
    else{
        return 1;
    }
}

// void slave_check_nack(){
// }

//ctrl_Reg1 is 0x2A register address

void set_ctrl_reg1(uint8_t data){ 
    // set the values of ctrl_reg1
    master_write_byte(0x2A, data);
}

void set_active_mode(void){
    set_ctrl_reg1(0x1);
}

void set_standby_mode(){
    // set the device in standby mode (can allow configs)
}

// send and read register but taken in argument is a char
void byte_read(char d){
    // read byte of data 
    if(d == 'x'){
        
    }
    else if(d == 'y'){
    
    }

};
