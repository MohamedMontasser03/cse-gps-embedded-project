#include <string.h>
#include "led.h"
#include "uart.h"
#include <math.h>
#include <stdlib.h>
#include "gps.h"   

#define M_PI 3.14159265358979323846
#define RADIUS 6371000 // radius of earth in meters

double to_degree(float raw_degree);
double to_radians(double degrees);
double distance(double lat1, double lon1, double lat2, double lon2);


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
