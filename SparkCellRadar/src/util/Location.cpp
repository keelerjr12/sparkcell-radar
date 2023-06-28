#include "Location.h"
#include "Math.h"
#include <cmath>
#include <numbers>

namespace SparkCell {

	// TODO: Create a type for the points
	float CalculateBearing(float lat_1, float lon_1, float lat_2, float lon_2) {
		const auto theta_1 = ToRadians(lat_1);
		const auto theta_2 = ToRadians(lat_2);
		const auto delta_1 = ToRadians(lat_2 - lat_1);
		const auto delta_2 = ToRadians(lon_2 - lon_1);

		const auto y = sin(delta_2) * cos(theta_2);
		const auto x = cos(theta_1) * sin(theta_2) - sin(theta_1) * cos(theta_2) * cos(delta_2);
		auto brng = atan2(y, x);

		brng = ToDegrees(brng);
		brng = static_cast<int>(brng + 360) % 360;

		return brng;
	}

	float CalculateRange(float lat_1, float lon_1, float lat_2, float lon_2) {
		const auto phi_1 = ToRadians(lat_1);
		const auto phi_2 = ToRadians(lat_2);
		const auto phi_delta = ToRadians(lat_2 - lat_1);
		const auto lambda_delta = ToRadians(lon_2 - lon_1);

		const auto a = (sin(phi_delta / 2) * sin(phi_delta / 2)) + (cos(phi_1) * cos(phi_2) * sin(lambda_delta / 2) * sin(lambda_delta / 2));
		const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

		const auto R = 3440.1;
		const float dist = R * c;

		return dist;
	}

}
