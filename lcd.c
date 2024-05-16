#include "lcd.h"

#include "uart.h"

void LCD_sendCommand(uint8_t command) {
    GPIO_PORTB_DATA_R &= 0xFE; /*Instruction Mode RS=0*/
    delayMillis(1);
    GPIO_PORTB_DATA_R |= 0X04; /*enable=1*/
    delayMillis(1);            // delay 40us
    /* out the highest 4 bits of the required command to the data bus D4 --> D7
     */
    GPIO_PORTB_DATA_R = ((GPIO_PORTB_DATA_R & 0x0F) | (command & 0xF0));
    delayMillis(1);
    GPIO_PORTB_DATA_R &= 0xFB; /*enable =0*/
    delayMillis(1);
    GPIO_PORTB_DATA_R |= 0X04; /*enable=1*/
    delayMillis(1);
    /* out the lowest 4 bits of the required command to the data bus D4 --> D7
     */
    GPIO_PORTB_DATA_R =
        ((GPIO_PORTB_DATA_R & 0x0F) | ((command << 4) & 0xF0)); /*write data*/
    delayMillis(1);
    GPIO_PORTB_DATA_R &= 0xFB; /*enable =0*/
    delayMillis(1);
}

