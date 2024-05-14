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

void LCD_init(void) {
    SYSCTL_RCGCGPIO_R |= 0x02;                    /* activate Port B */
    while ((SYSCTL_PRGPIO_R &= 0x00000002) == 0); /*waite to activate*/
    GPIO_PORTB_AMSEL_R &= 0;                      /* Disable Analog on PORTB*/
    GPIO_PORTB_PCTL_R &=
        0x00000000; /* Clear PMCx bits for  PORTB to use it as GPIO pin */
    GPIO_PORTB_DIR_R |= 0xFF;  /* Configure PORTB as output pin */
    GPIO_PORTB_AFSEL_R &= 0x0; /* Disable alternative function on PORTB */
    GPIO_PORTB_DEN_R |= 0xFF;  /* Enable Digital I/O onPORTB */
    GPIO_PORTB_DATA_R &= 0x0;  /* Clear data in PORT B */

    delayMillis(20);        // delay 20ms
    LCD_sendCommand(0X33);  // wakeup
    delayMillis(5);         // DELAY 5mS
    LCD_sendCommand(0X32);  // wakeup
    LCD_sendCommand(0X28);  /* Select 4-bit Mode of LCD */
    LCD_sendCommand(0X06);  /* clear whatever is written on display */
    LCD_sendCommand(0X01);  /* shift cursor right */
    LCD_sendCommand(0X0F);  /* Enable Display and cursor blinking */
    LCD_sendCommand(0x02);
}

void LCD_displayCharacter(uint8_t data) {
    GPIO_PORTB_DATA_R |=
        0x01; /* Instruction Mode RS=1 /write data to LCD so RW=0/ENABLE=0 */
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | (data & 0xF0);
    GPIO_PORTB_DATA_R |= 0X04; /*enable=1*/
    delayMillis(40);
    GPIO_PORTB_DATA_R &= 0xFB; /*enable =0*/
    GPIO_PORTB_DATA_R =
        ((GPIO_PORTB_DATA_R & 0x0F) | ((data << 4) & 0xF0)); /*write data*/
    GPIO_PORTB_DATA_R |= 0X04;                               /*enable=1*/
    delayMillis(40);
    GPIO_PORTB_DATA_R &= 0xFB; /*enable =0*/
    delayMillis(40);
}

void LCD_displayString(const char* Str) {
    uint8_t i = 0;
    while (Str[i] != '\0') {
        if (i == 16) {
            LCD_sendCommand(0xC0);
        } else if (i == 32) {
            LCD_sendCommand(0x01);
            LCD_sendCommand(0x80);
        }
        LCD_displayCharacter(Str[i]);
        i++;
    }
}

void LCD_clear(void) {
    LCD_sendCommand(0x01);
    delayMillis(2);
}

void LCD_displayfloat(float data) {
    char str[7];
    int i = 0;
    sprintf(str, "%f", data);  // used to convert flaot to string
    LCD_sendCommand(1);        /* clear display */
    LCD_sendCommand(0x80);     /* LCD cursor location */

    while (str[i] != '\0') {
        LCD_displayCharacter(str[i]);
        i++;
    }
    delayMillis(500);  // delay 500ms
}