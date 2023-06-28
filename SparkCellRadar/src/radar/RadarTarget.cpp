#include "RadarTarget.h"
#include "../util/Location.h"
#include "Radar.h"
#include <SimulatedRadarTypes.h>

namespace SparkCell {

	RadarTarget::RadarTarget(const Aircraft& host, const Aircraft& tgt)
		: host_(host), target_(tgt) { } 

	float RadarTarget::Airspeed() const {
		return target_.airspeed();
	}

	float RadarTarget::Altitude() const {
		return target_.alt();
	}

	float RadarTarget::Bearing() const {
		const auto brng = CalculateBearing(host_.lat(), host_.lon(), target_.lat(), target_.lon());
		return brng;
	}
	float RadarTarget::Range() const {
		const auto rng = CalculateRange(host_.lat(), host_.lon(), target_.lat(), target_.lon());
		return rng;
	}

	float RadarTarget::Heading() const {
		return target_.heading();
	}

	float RadarTarget::AA() const {
		return Heading() - Bearing();
	}

	float RadarTarget::ATA() const {
		auto ata = static_cast<int>(Bearing() - host_.heading() + 360) % 360;

		if (ata > 180)
			ata -=  360;

		return ata;
	}

}
