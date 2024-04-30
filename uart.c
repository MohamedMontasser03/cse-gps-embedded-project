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

void Uart5_init(void){													// PE Rx -> 4 ; Tx -> 5
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R5;							// Enable UART5 clock   0010 0000
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;							// Enable port E clock
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0){};
	UART5_CTL_R &= ~UART_CTL_UARTEN;									// Disable UART5
	UART5_IBRD_R = 1000000 / 9600;
	UART5_FBRD_R =  1000000 % 9600 / 9600.0 * 64 + 0.5;
	UART5_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN) & ~UART_LCRH_PEN; // 8 bit, no parity, 1 stop, FIFOs
	UART5_CTL_R = UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE;		// Enable UART5, Rx, Tx
	GPIO_PORTE_AFSEL_R |= (1 << 4) | (1 << 5);							// Alternate function PE4 - PE5   0011 0000
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) | 0x00110000;	// Set PCTL for PE4-5 to UART function (Ref P 651)
	GPIO_PORTE_DEN_R |= (1 << 4) | (1 << 5);							// Enable digital pins PE4-PE5   0011 0000
	GPIO_PORTE_AMSEL_R &= ~(1 << 4) | (1 << 5); 						// Disable analog function PE4-PE5   0011 0000
}
void uart5_send_byte(uint8_t c)
{
	while((UART5_FR_R & 0x20) != 0);  
	UART5_DR_R = c; 					
}



void Uart5_output_string(char* pt){
	while(*pt){
		uart5_send_byte(*pt);
		pt++;
	}
}



void uart2_send_byte(uint8_t c)
{
	while((UART2_FR_R & 0x20) != 0);  
	UART2_DR_R = c; 					
}

uint8_t uart2_read_byte(void)
{
	uint8_t c;
	
	while((UART2_FR_R & 0x10) != 0);  
	c = UART2_DR_R; 					
	
	return c; 
}


