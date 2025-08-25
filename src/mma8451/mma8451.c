#include "mma8451.h"

// user adjustable
uint8_t sa0 = 0;

// TODO: at some point, split up helper functions with actual user functions

// TODO: allow for config of ctrlreg and then set active mode with bitwise or 

// TODO: if the program gets hung on an infinite while loop, maybe incorporate a system reset after logging?

void init_mma8451(void){
    // initialize the driver

    init_i2c();
}

void master_send_start(void){
    I2C1_CR1 |= (0x1 << 8);
    //wait for start bit registered
    while(!(I2C1_SR1 & 0x1)){}
}

void master_send_stop(void){
    I2C1_CR1 |= (0x1 << 9);
}

void master_send_ack(void){
    // send ack from master
    I2C1_CR1 |= (0x1 << 10);
}

void master_clear_addr(void){
    // clear the address by reading sr1 and then sr2
    volatile uint8_t buffer = I2C1_SR1;
    buffer = I2C1_SR2;
}

void master_check_addr_sent(void){
    while(!(I2C1_SR1 & (0x1 << 1))){}
}

void master_send_nack(void){
    // send nack from master
    I2C1_CR1 &= ~(0x1 << 10);
}

void master_wait_txe(void){
    // wait for txe (transmit) data register to be ready (empty)
    while(!(I2C1_SR1 & (0x1 << 7))){}
}

void master_wait_rxne(void){
    // wait for the rxne (receiver) data register to be ready (filled)
    while(!(I2C1_SR1 & (0x1 << 6))){}
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

    // send slave address
    I2C1_DR = addr;
    master_check_addr_sent();
    master_clear_addr();

    // send target reg
    I2C1_DR = targetreg;
    master_wait_txe();

    // send data 
    I2C1_DR = data;
    master_wait_btf();

    master_send_stop();
}

uint8_t master_read_byte(uint8_t targetreg){
    uint8_t readval;
    uint8_t waddr;
    uint8_t raddr;
    // sa0 = 3.3v 
    if(sa0){
        waddr = (0x1D << 1);
        raddr = (0x1D << 1) | 0x1;
    }
    // sa0 = GND
    else{
        waddr = (0x1C << 1);
        raddr = (0x1C << 1) | 0x1;
    }

    // send slave address (w)
    master_send_start();

    I2C1_DR = waddr;
    master_check_addr_sent();
    master_clear_addr();

    // send register to read from
    I2C1_DR = targetreg;
    master_wait_txe();

    master_send_ack();

    //repeated start condition, then send slave address (r)
    master_send_start();
    I2C1_DR = raddr;
    master_check_addr_sent();
    master_clear_addr();

    // send nack
    master_send_nack();    

    // read data register
    master_wait_rxne();
    readval = I2C1_DR;

    // send stop
    master_send_stop();
    return readval;
}

void master_read_bytes(uint8_t* buffer, uint8_t targetreg, uint8_t rlen){   
    // fill buffer with multibyte read of rlen size
    uint8_t waddr;
    uint8_t raddr;
    // sa0 = 3.3v 
    if(sa0){
        waddr = (0x1D << 1);
        raddr = (0x1D << 1) | 0x1;
    }
    // sa0 = GND
    else{
        waddr = (0x1C << 1);
        raddr = (0x1C << 1) | 0x1;
    }

    // send slave address (w)
    master_send_start();

    I2C1_DR = waddr;
    master_check_addr_sent();
    master_clear_addr();

    // send register to read from
    I2C1_DR = targetreg;
    master_wait_txe();

    //repeated start condition, then send slave address (r)
    master_send_start();
    I2C1_DR = raddr;
    master_check_addr_sent();
    master_clear_addr();

    // set ack
    master_send_ack();

    // loop read and ack (unless last byte, then nack and stop)
    for(int i = 0; i < rlen; i++){
        master_wait_rxne();
        buffer[i] = I2C1_DR;
        // if last byte
        if(i == rlen - 1){
            master_send_nack();
            master_send_stop();
        }
        else{
            master_send_ack();
        }
    }
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

void master_write_bytes(uint8_t targetreg, uint8_t* d_arr, uint8_t arrlen){
    // multi byte write
    // single byte write, a for if sa0 is GND or 3.3v
    uint8_t addr;
    // sa0 = 3.3v 
    if(sa0) addr = (0x1D << 1);
    // sa0 = GND
    else addr = (0x1C << 1);

    master_send_start();

    // send slave address
    I2C1_DR = addr;
    master_check_addr_sent();
    master_clear_addr();

    // send target reg
    I2C1_DR = targetreg;
    master_wait_txe();

    // multi byte write
    for(int i = 0; i < arrlen; i++){
        I2C1_DR = d_arr[i];
        master_wait_btf();
    }

    master_send_stop();
}

uint8_t master_wait_ack(void){
    // wait for address sent confirmation
    while(!(I2C1_SR1 & (0x1 << 1))){}
    
    // todo: maybe add a small delay if this somehow doesn't work during testing

    // check for nack, bit is set 1 if nack
    if(I2C1_SR1 & (0x1 << 10)){
        return 0;
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
    // standby -> active
    set_ctrl_reg1(0x1);
}

void set_standby_mode(void){
    // set the device in standby mode (can allow configs)
}
