#include "Math.h"
#include <numbers>

namespace SparkCell {

	float ToRadians(float degs) {
		return degs * std::numbers::pi / 180;
	}

	float ToDegrees(float rads) {
		return rads * 180 / std::numbers::pi;
	}

}