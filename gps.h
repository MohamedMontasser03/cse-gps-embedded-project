#ifndef GPS_H_INCLUDED
#define GPS_H_INCLUDED
void GPS_read(void);
void GPS_format(void);
float to_degree(float raw_degree);
double to_radians(double degrees);
double distance(double lat1, double lon1, double lat2, double lon2);

#endif
