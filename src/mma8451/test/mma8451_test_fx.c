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
    set_active_mode_only();

    while(1){  
        char accel_str[16];
        int16_t buffer = read_accel(axis, 'r');
        sprintf(accel_str, "%i\r\n\r\n", buffer);   

        //sleep for half a sec
        uart_print(accel_str);
        systick_sleep(200);
    }
};

void test_read_accel_fmode(char axis){
// testing fast mode 
    init_mma8451();
    // set active mode and fast mode
    configure_ctrl_reg1(1, 1, 0, 0, 0);

    while(1){
        int8_t data = (int8_t) read_accel(axis, 'f');
        char buffer[64]; 
        sprintf(buffer, "Raw Fast Mode %c-Axis: %i\r\n", axis, data);
        uart_print(buffer);
        systick_sleep(500); 
    }
}
//* left off writing out test function below

void test_read_accel_converted(char axis){
// testing proper acceleration conversion
    init_mma8451();
    set_active_mode_only();

    while(1){
        int accel = read_accel_converted(axis, 'r', 2);
        char buffer[64];
        sprintf(buffer, "%c-Axis is %i m/s^2\r\n", axis, accel);
        uart_print(buffer);
        systick_sleep(500);
    }
}

void test_read_all_accel(void){

    init_mma8451();
    set_active_mode_only();

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
    set_active_mode_only();
}

