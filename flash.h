#ifndef _TM4C_H
#define _TM4C_H
#include "tm4c123gh6pm.h"
#endif
#ifndef _STD_INT_H
#define _STD_INT_H
#include <stdint.h>
#endif
#ifndef _FLASH_H
#define _FLASH_H
#define FLASH_BASE_ADDR ((volatile uint32_t*)0x00020000)

void Flash_Enable(void);
void Flash_Read(void* data, int wordCount, uint32_t offset);
#endif

