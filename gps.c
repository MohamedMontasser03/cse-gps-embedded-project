#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "gps.h"
#include "led.h"

char GPS_logname[]="$GPRMC";
char GPS[80];
char GPS_formated[12][20];
char * token;
float currentLat;
float currentLong;


//extract $GPRMC message content
void GPS_read(void){
	char i=0;
	char counter=0;

	//wait till $GPRMC message
	do{
			while(uart2_read_byte() != GPS_logname[i]){
					i=0;   
			}
			i++;
		}while(i!=6);
	
		
		
	//extract GPRMC message content
	do{	
			GPS[counter]=uart2_read_byte();
			counter++;
	  }while(GPS[counter-1]!= '*');
}

