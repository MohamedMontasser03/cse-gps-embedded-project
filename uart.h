#ifndef _TM4C_H
#define _TM4C_H
#include "tm4c123gh6pm.h"
#endif
#ifndef _STDINT_H
#define _STDINT_H
#include <stdint.h>
#endif
#ifndef _UART_H
#define _UART_H

void uart2_init();

void uart2_send_byte(uint8_t c);

uint8_t uart2_read_byte(void);
void Uart0_init(void);
void uart0_send_byte(uint8_t c);
void Uart0_output_string(char* pt);

void SYSTICKTIMER_init(void);
void SYSTICK_wait1ms(void);
void delayMillis(uint32_t delay);

#endif
