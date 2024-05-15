#include "gps.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "led.h"
#include "uart.h"

char GPS_logname[] = "$GPRMC";
char GPS[80];
char GPS_formated[12][20];
char* token;
float currentLat;
float currentLong;

// extract $GPRMC message content
void GPS_read(void) {
    char i = 0;
    char counter = 0;

    // wait till $GPRMC message
    do {
        while (uart2_read_byte() != GPS_logname[i]) {
            i = 0;
        }
        i++;
    } while (i != 6);

    // extract GPRMC message content
    do {
        GPS[counter] = uart2_read_byte();
        counter++;
    } while (GPS[counter - 1] != '*');
}

// extract Latitude  and Longitude of the $GPRMC message
void GPS_format(void) {
    char noofStrings = 0;
    token = strtok(GPS, ",");

    do {
        strcpy(GPS_formated[noofStrings], token);
        token = strtok(NULL, ",");
        noofStrings++;
    } while (token != NULL);

    if (strcmp(GPS_formated[1], "A") == 0) {
        if (strcmp(GPS_formated[3], "N") == 0)
            currentLat = atof(GPS_formated[2]);
        else
            currentLat = -(atof(GPS_formated[2]));

        if (strcmp(GPS_formated[5], "E") == 0)
            currentLong = atof(GPS_formated[4]);
        else
            currentLong = -(atof(GPS_formated[4]));
    }
}

float to_degree(float raw_degree) {  // gps output to degrees
    int dd = (int)(raw_degree / 100);
    float mm = raw_degree - (dd * 100);
    float degree = dd + (mm / 60);
    return degree;
}

double to_radians(double degrees) {  // degree to rad
    return degrees * M_PI / 180.0;
}

double distance(double lat1, double lon1, double lat2,
                double lon2) {  // calc distance between 2 coordinates in M
    double dlat = to_radians(lat2 - lat1);
    double dlon = to_radians(lon2 - lon1);
    double a = pow(sin(dlat / 2), 2) + cos(to_radians(lat1)) *
                                           cos(to_radians(lat2)) *
                                           pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return RADIUS * c;
}
