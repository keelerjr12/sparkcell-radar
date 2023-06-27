#include "RadarTarget.h"
#include "Radar.h"
#include <SimulatedRadarTypes.h>
#include <numbers>

namespace SparkCell {

	RadarTarget::RadarTarget(const Aircraft& host, const Aircraft& tgt)
		: mHost(host), mTarget(tgt) { } 

	float radians(float degs) {
		return degs * std::numbers::pi / 180;
	}

	float degrees(float rads) {
		return rads * 180 / std::numbers::pi;
	}

	float RadarTarget::getBearing() const {
		const auto theta_1 = radians(mHost.lat());
		const auto theta_2 = radians(mTarget.lat());
		const auto delta_1 = radians(mTarget.lat() - mHost.lat());
		const auto delta_2 = radians(mTarget.lon() - mHost.lon());

		const auto y = sin(delta_2) * cos(theta_2);
		const auto x = cos(theta_1) * sin(theta_2) - sin(theta_1) * cos(theta_2) * cos(delta_2);
		auto brng = atan2(y, x);

		brng = degrees(brng);
		brng = static_cast<int>(brng + 360) % 360;

		return brng;
	}
	float RadarTarget::getRange() const {
		const auto phi_1 = mHost.lat() * (std::numbers::pi / 180);
		const auto phi_2 = mTarget.lat() * (std::numbers::pi / 180);
		const auto phi_delta = (mTarget.lat() - mHost.lat()) * (std::numbers::pi / 180);
		const auto lambda_delta = (mTarget.lon() - mHost.lon()) * (std::numbers::pi / 180);

		const auto a = (sin(phi_delta / 2) * sin(phi_delta / 2)) + (cos(phi_1) * cos(phi_2) * sin(lambda_delta / 2) * sin(lambda_delta / 2));
		const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

		const auto R = 3440.1;
		const float dist = R * c;

		return dist;
	}

	float RadarTarget::getHeading() const {
		return mTarget.heading();
	}

	float RadarTarget::getAA() const {
		return getHeading() - getBearing();
	}

	float RadarTarget::getATA() const {
		auto ata = static_cast<int>(getBearing() - mHost.heading() + 360) % 360;

		if (ata > 180)
			ata -=  360;

		return ata;
	}

}
