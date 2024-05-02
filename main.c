#include <string.h>
#include "led.h"
#include "uart.h"
#include <math.h>
#include <stdlib.h>
#include "gps.h"   

#define M_PI 3.14159265358979323846
#define RADIUS 6371000 // radius of earth in meters

double tot_distance=0;

// points coordinates
double lat1=0;
double long1=0;
extern float currentLat;
extern float currentLong;

void portF_init(void);
double to_degree(float raw_degree);
double to_radians(double degrees);
double distance(double lat1, double lon1, double lat2, double lon2);
double approximate( double a, float d);
void UART0SendFloat(float num);

int main(void){
	
	
	SYSTICKTIMER_init();
	portF_init();
	Uart5_init();
  	uart2_init();
	

	RGB_set(0x2);
	delayMillis(3000);
	RGB(0X00);
	delayMillis(1000*20);
	RGB(0x0E);
	delayMillis(3000);
	RGB(0X00);
	
	RGB(RED_LED);
	
	//get start point
	GPS_read();
	GPS_format();
	lat1 = to_degree(currentLat);
	long1 = to_degree(currentLong);

		while(1){
		GPS_read();
		GPS_format();
		
		currentLat = to_degree(currentLat);
		currentLong = to_degree(currentLong);
			
		tot_distance += distance(lat1, long1, currentLat, currentLong);
			lat1 = currentLat;
			long1 = currentLong;
			
			if(tot_distance >=100 || (GPIO_PORTF_DATA_R & SW1)==0){
				RGB(GREEN_LED);
				break;
		}
	}	
}

void portF_init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;// Enable port F clock
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;		// Unlock PortF PF0
	GPIO_PORTF_CR_R = 0x1F;					// Allow changes to PF1-3
	GPIO_PORTF_DIR_R = 0x0E;				// PF3,PF2,PF1 output
	GPIO_PORTF_AFSEL_R = 0x00;				// No alternate function
	GPIO_PORTF_PCTL_R = 0x00000000;			// GPIO clear bit PCTL
	GPIO_PORTF_DEN_R = 0x1F;				// Enable digital pins PF1-PF3
	GPIO_PORTF_AMSEL_R = 0x00;				// Disable analog function
	GPIO_PORTF_PUR_R = 0x11;        // enable pull-up on PF0 and PF4
}


double to_degree(float raw_degree){  //gps output to degrees
	  int dd = (int) (raw_degree / 100);
    double mm = raw_degree - (dd * 100);
    double degree = dd + (mm / 60);
    return degree;
}


double to_radians(double degrees) {   // degree to rad
    return degrees * M_PI / 180.0;
}


double distance(double lat1, double lon1, double lat2, double lon2) {  //calc distance between 2 coordinates in M
    double dlat = to_radians(lat2 - lat1);
    double dlon = to_radians(lon2 - lon1);
    double a = pow(sin(dlat / 2), 2) + cos(to_radians(lat1)) * cos(to_radians(lat2)) * pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return RADIUS * c;
}


double approximate( double a, float d) {
	return (int)(a/d +0.5)*d;
}


void UART0SendFloat(float num) {
    int i;
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%f", num);

   for( i=0 ; buffer[i] != '\0'; i++) {
        while((UART0_FR_R & UART_FR_TXFF)==UART_FR_TXFF); // Wait until the transmitter is not full
        UART0_DR_R = buffer[i]; // Transmit the character
    }
}
