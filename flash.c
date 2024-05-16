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
int Flash_Erase(int blockCount) {
    // Make sure Enable was called.
    int i;
    if (flashKey_ == 0) {
        return -1;
    }

    for (i = 0; i < blockCount; i++) {
        // Clear then set the OFFSET (17:0) with the write address.
        FLASH_FMA_R &= 0xFFFC0000;

        // Blocks are erased on 1KiB boundaries, so multiply the index by 1024
        //	and add this to the base address.
        FLASH_FMA_R |= ((uint32_t)FLASH_BASE_ADDR) + (i * 1024);

        // Set the ERASE command bit.
        FLASH_FMC_R = (flashKey_ << 16) | 0x2;

        // Poll the ERASE bit until it is cleared.
        while (FLASH_FMC_R & 0x2) {
        }
    }

    return 0;
}
