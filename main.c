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

double to_degree(float raw_degree);
double to_radians(double degrees);
double distance(double lat1, double lon1, double lat2, double lon2);


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
			
			if(tot_distance >=5 || (GPIO_PORTF_DATA_R & SW1)==0){
				RGB(GREEN_LED);
				break;
		}
	}	
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
