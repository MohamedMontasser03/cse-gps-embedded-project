#ifndef _TM4C_H
#define _TM4C_H
#include "tm4c123gh6pm.h"
#endif
#ifndef _STD_INT_H
#define _STD_INT_H
#include <stdint.h>
#endif
#ifndef _LCD_H
#define _LCD_H

#define LCD_SECOND_LINE 0xC0

void LCD_sendCommand(uint8_t command);
void LCD_init(void);
void LCD_displayCharacter(uint8_t data);
void LCD_displayString(const char* Str);
void LCD_clear(void);
void LCD_displayfloat(float data);

#endif