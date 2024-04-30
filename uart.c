#include "uart.h"


void uart2_init(){ // for gps
    unsigned BRD;

    SYSCTL_RCGCUART_R |= 0X04; // activate UART2
    SYSCTL_RCGCGPIO_R |= 0X08; //  activate port D

    UART2_CTL_R &= ~(0X0001);   // disable UART
    UART2_IBRD_R = 104;
    UART2_FBRD_R = 11;

    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;  // Unlock port D
    GPIO_PORTD_CR_R |= 0xC0;  // Allow changes to PD7-PD6
    GPIO_PORTD_AFSEL_R |= 0XC0; // enable alt function PD7, PD6
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0X00FFFFFF) | 0X11000000; // configure uart for pa0,pa1

    UART2_CC_R = 0; 	   // use system clock
    UART2_LCRH_R = 0x60; // 8-bit word length, no Fifo , no parity, 1 stop bit
    UART2_CTL_R = 0X0301;  // enable RXE,TXE AND UART

    GPIO_PORTD_DEN_R |= 0XC0;  // enable digital IO on PD6,PD7
    GPIO_PORTD_AMSEL_R &= ~0XC0; // disable analog function on PD6, PD7

    delayMillis(1);

}


