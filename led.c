#include "led.h"

void RGB_set(char mask) {
    mask &= RGB_LED;
    GPIO_PORTF_DATA_R = mask;
}

void RGB_clear(char mask) {
    mask &= RGB_LED;
    GPIO_PORTF_DATA_R &= ~mask;
}

void RGB(char colour) {
    GPIO_PORTF_DATA_R &= ~RGB_LED;  // Stop all active LEDs
    GPIO_PORTF_DATA_R |= colour;    // Turn on this led
}
