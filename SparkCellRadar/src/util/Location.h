#ifndef LOCATION_H
#define LOCATION_H

namespace SparkCell {

	float CalculateBearing(float lat_1, float lon_1, float lat_2, float lon_2);

	float CalculateRange(float lat_1, float lon_1, float lat_2, float lon_2);

}
 
#endif // LOCATION_H