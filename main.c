#include "flash.h"
#include "gps.h"
#include "lcd.h"
#include "led.h"
#include "uart.h"

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

void savedDataProcedure(void);

int main(void) {
    SYSTICKTIMER_init();
    portF_init();
    Flash_Enable();
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

    // some delays to make sure the gps module is ready
    RGB_set(RED_LED);
    delayMillis(3000);
    LCD_clear();
    LCD_displayString("Initializing GPS module");
    RGB(0X00);
    delayMillis(1000 * 20);
    RGB(0x0E);
    delayMillis(3000);
    RGB(RED_LED);

    // get start point
    LCD_clear();
    LCD_displayString("Getting start point");
    GPS_read();
    GPS_format();
    lat1 = to_degree(currentLat);
    long1 = to_degree(currentLong);
    points[0][0] = lat1;
    points[0][1] = long1;
    current_point++;

    while (1) {
        LCD_clear();
        LCD_displayString("LAT: ");
        LCD_displayfloat(currentLat);
        LCD_sendCommand(LCD_SECOND_LINE);
        LCD_displayString("LONG: ");
        LCD_displayfloat(currentLong);

        GPS_format();

        currentLat = to_degree(currentLat);
        currentLong = to_degree(currentLong);

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
            LCD_clear();
            LCD_displayString("Saving points to flash");
            Flash_Erase(2);
            Flash_Write(&current_point, 1, 0);
            Flash_Write(points, current_point * 2, 1);
            LCD_clear();
            LCD_displayString("Points saved to flash");
            break;
        }
        delayMillis(2000);
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
