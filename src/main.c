#include "uart.h"
#include "systick.h"

int main(){
    init_systick();
    init_uart();

    while(1){
        uart_print("testing!\r\n");
        systick_sleep(1000);    
    }
};