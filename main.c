#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "gps.h"
#include "led.h"
#include "uart.h"
#include "lcd.h"
#include "flash.h"


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

    LCD_clear();
    LCD_displayString("Initializing GPS module");
	
    RGB_set(0x2);
    delayMillis(3000);
    RGB(0X00);
    delayMillis(1000 * 20);
    RGB(0x0E);
    delayMillis(3000);
    RGB(0X00);

    RGB(RED_LED);
// get start point
    LCD_clear();
    LCD_displayString("Getting start point");

    while(lat1 == 0 || long1 == 0) {
    	GPS_read();
	GPS_format();
	lat1 =(int) (to_degree(currentLat) * 100000);
	lat1=(float) lat1/ 100000;
	long1 =(int) (to_degree(currentLong) * 100000);
	long1=(float) long1/ 100000;
    }
    points[0][0] = lat1;
    points[0][1] = long1;
    current_point++;

    while (1) {
        GPS_read();
        GPS_format();

    	currentLat =(int)((to_degree(currentLat)) *100000);
	currentLat= (float) currentLat/ 100000;
        currentLat =(int)((to_degree(currentLong)) *100000);
	currentLat= (float) currentLong/ 100000;
	    
	LCD_clear();
        LCD_displayString("distance: ");
        LCD_displayfloat(tot_distance);
	    
        tot_distance += distance(lat1, long1, currentLat, currentLong);
        lat1 = currentLat;
        long1 = currentLong;

	points[current_point][0] = lat1;
        points[current_point][1] = long1;
        current_point++;

        if (tot_distance >= 100 || (GPIO_PORTF_DATA_R & SW1) == 0 ||
            current_point == 200) {
            RGB(GREEN_LED);
            LCD_clear();
            LCD_displayString("Total distance: ");
            LCD_sendCommand(LCD_SECOND_LINE);
            LCD_displayfloat(tot_distance);
            LCD_displayString(" m");
	    delayMillis(2000);
            LCD_clear();
            LCD_displayString("Saving points to flash");
            Flash_Erase(2);
            Flash_Write(&current_point, 1, 0);
            Flash_Write(points, current_point * 2, 1);
            LCD_clear();
            LCD_displayString("Points saved to flash");
            break;
        }
    }
    savedDataProcedure();

    LCD_clear();
    // end program
    LCD_displayString("Thank you for using our app");
    while (1) {
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




