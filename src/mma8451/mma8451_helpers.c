#include "mma8451_helpers.h"

// user adjustable value
uint8_t sa0 = 0;

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

void master_send_nack(void){
    // send nack from master
    I2C1_CR1 &= ~(0x1 << 10);
}

void master_check_addr_sent(void){
    while(!(I2C1_SR1 & (0x1 << 1))){}
}

void master_clear_addr(void){
    // clear the address by reading sr1 and then sr2
    volatile uint8_t buffer = I2C1_SR1;
    buffer = I2C1_SR2;
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

// * not currently in use
// uint8_t master_wait_ack(void){
//     // wait for address sent confirmation
//     while(!(I2C1_SR1 & (0x1 << 1))){}
    
//     // todo: maybe add a small delay if this somehow doesn't work during testing

//     // check for nack, bit is set 1 if nack
//     if(I2C1_SR1 & (0x1 << 10)){
//         return 0;
//     }
//     else{
//         return 1;
//     }
// }