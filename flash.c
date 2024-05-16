#include "flash.h"

static uint16_t flashKey_ = 0;
void Flash_Enable(void) {
    // If the KEY flag (bit 4) is set, then the key is 0xA442.
    //	Otherwise, the key is 0x71D5 (pg. 583 of the TM4C123 datasheet).
    if (FLASH_BOOTCFG_R & 0x10) {
        flashKey_ = 0xA442;
    } else {
        flashKey_ = 0x71D5;
    }
}

void Flash_Read(void* data, int wordCount, uint32_t offset) {
    int i;
    // Copy the count of bytes into the target data buffer...
    for (i = 0; i < wordCount; i++) {
        ((uint32_t*)data)[i] = FLASH_BASE_ADDR[i + offset];
    }
}
