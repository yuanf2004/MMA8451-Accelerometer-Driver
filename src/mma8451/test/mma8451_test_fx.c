#include "mma8451_test_fx.h"

void test_read_byte(void){
    // test the read byte with MSB of x-axis
    init_mma8451();
    uint16_t r = master_read_byte(0x01);
    while(1){
    }
};

void test_read_accel(char axis){
    // test to see if acceleration is being properly read 
    init_mma8451();

    while(1){  
        char accel_str[16];
        uint16_t buffer = read_accel(axis);
        sprintf(accel_str, "%u\r\n", buffer);   

        //sleep for half a sec
        uart_print(accel_str);
        systick_sleep(500);
    }
};

void test_set_active_mode(void){
    init_mma8451();
    set_active_mode();
}