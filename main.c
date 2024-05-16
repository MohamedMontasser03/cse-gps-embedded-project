#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "gps.h"
#include "led.h"
#include "uart.h"

#define M_PI 3.14159265358979323846
#define RADIUS 6371000  // radius of earth in meters

double tot_distance = 0;

// points coordinates
double lat1 = 0;
double long1 = 0;
extern float currentLat;
extern float currentLong;
// create a buffer to store points coordinates
// with a size of 200 points so 2 * 200 * 4
// this buffer is an array of 200 points each is an array of 2 elements
float points[200][2];
int current_point = 0;

void portF_init(void);

// create a buffer to store points coordinates
// with a size of 200 points so 2 * 200 * 4
// this buffer is an array of 200 points each is an array of 2 elements
float points[200][2];
int current_point = 0;

void savedDataProcedure(void);

int main(void) {
    SYSTICKTIMER_init();
    portF_init();
    uart2_init();
    uart0_init();
    LCD_init();
    LCD_clear();
    LCD_displayString("Starting the path tracking app");
	
    Flash_Read(&current_point, 1, 0);
    if (current_point < 200 && current_point > 0) {
        LCD_clear();
        LCD_displayString("Reading points from flash");
        Flash_Read(points, current_point * 2, 1);
        savedDataProcedure();
    }

    RGB_set(0x2);
    delayMillis(3000);
    RGB(0X00);
    delayMillis(1000 * 20);
    RGB(0x0E);
    delayMillis(3000);
    RGB(0X00);

    RGB(RED_LED);

    // get start point
    GPS_read();
    GPS_format();
    lat1 = to_degree(currentLat);
    long1 = to_degree(currentLong);

    while (1) {
        GPS_read();
        GPS_format();

        currentLat = to_degree(currentLat);
        currentLong = to_degree(currentLong);

        tot_distance += distance(lat1, long1, currentLat, currentLong);
        lat1 = currentLat;
        long1 = currentLong;

        if (tot_distance >= 100 || (GPIO_PORTF_DATA_R & SW1) == 0) {
            RGB(GREEN_LED);
            break;
        }
    }
}

void savedDataProcedure(void) {
    int i;
    while (1) {
        LCD_clear();
        LCD_displayString("Waiting for signal to send data");
        LCD_clear();
        LCD_displayString("Press SW1 to abort");

        // if we recieve "u" from uart0 we will send the data
        if ((UART0_FR_R & 0x10) == 0 && uart0_read_byte() == 'u') {
            LCD_clear();
            LCD_displayString("Sending data");
            // send number of points
            UART0SendInt(current_point);
            for (i = 0; i < current_point; i++) {
                UART0SendFloat(points[i][0]);
                UART0SendFloat(points[i][1]);
            }
            break;
        }

        if ((GPIO_PORTF_DATA_R & SW1) == 0) {
            LCD_clear();
            LCD_displayString("Aborted, Clearing data");
            Flash_Erase(2);
            break;
        }
    }
}

void portF_init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;  // Enable port F clock
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {
    };
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  // Unlock PortF PF0
    GPIO_PORTF_CR_R = 0x1F;             // Allow changes to PF1-3
    GPIO_PORTF_DIR_R = 0x0E;            // PF3,PF2,PF1 output
    GPIO_PORTF_AFSEL_R = 0x00;          // No alternate function
    GPIO_PORTF_PCTL_R = 0x00000000;     // GPIO clear bit PCTL
    GPIO_PORTF_DEN_R = 0x1F;            // Enable digital pins PF1-PF3
    GPIO_PORTF_AMSEL_R = 0x00;          // Disable analog function
    GPIO_PORTF_PUR_R = 0x11;            // enable pull-up on PF0 and PF4
}




