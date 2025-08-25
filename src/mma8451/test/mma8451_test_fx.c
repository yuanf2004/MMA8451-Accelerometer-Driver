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
    set_active_mode();

    while(1){  
        char accel_str[16];
        int16_t buffer = read_accel(axis, 'r');
        sprintf(accel_str, "%i\r\n\r\n", buffer);   

        //sleep for half a sec
        uart_print(accel_str);
        systick_sleep(200);
    }
};

void test_read_all_accel(void){

    init_mma8451();
    set_active_mode();

    while(1){
        int16_t t_arr[3];
        read_all_accel(t_arr, 'r');
        
        char msg[128];
        sprintf(msg, "X axis: %i,\r\n Y axis: %i,\r\n Z axis: %i\r\n\r\n", t_arr[0], t_arr[1], t_arr[2]);
        
        uart_print(msg);
        systick_sleep(1000);
    }

};

void test_set_active_mode(void){
// * no while loop
    init_mma8451();
    set_active_mode();
}

