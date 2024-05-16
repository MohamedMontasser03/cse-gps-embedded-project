#ifndef GPS_H_INCLUDED
#define GPS_H_INCLUDED
#define M_PI 3.14159265358979323846
#define RADIUS 6371000  // radius of earth in meters

void GPS_read(void);
void GPS_format(void);
double to_degree(float raw_degree);
double to_radians(double degrees);
double distance(double lat1, double lon1, double lat2, double lon2);

#endif
