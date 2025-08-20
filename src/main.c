#include "uart/uart.h"
#include "systick/systick.h"

int main(){

    // initialization
    init_systick();
    init_uart();

    while(1){
        uart_print("testing!\r\n");
        systick_sleep(1000);    
    }
};