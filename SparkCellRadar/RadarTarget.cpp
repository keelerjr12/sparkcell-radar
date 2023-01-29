#include "RadarTarget.h"
#include "Radar.h"
#include <SimulatedRadarTypes.h>
#include <numbers>

SparkCell::RadarTarget::RadarTarget(const ::Radar::LLA& radarLLA, const ::Radar::PBH& radarPBH, const ::Radar::LLA& tgtLLA, const ::Radar::PBH& tgtPBH, const P3D::P3DDXYZ& tgtVel)
	: mRadarLLA(radarLLA), mRadarPBH(radarPBH), mTgtLLA(tgtLLA), mTgtPBH(tgtPBH), mTgtVel(tgtVel) { } 

float radians(float degs) {
	return degs * std::numbers::pi / 180;
}

float degrees(float rads) {
	return rads * 180 / std::numbers::pi;
}

float SparkCell::RadarTarget::getAirspeed() const {
	return sqrt(mTgtVel.dX * mTgtVel.dX + mTgtVel.dY * mTgtVel.dY + mTgtVel.dZ * mTgtVel.dZ); // TODO: this may need to be in 3D?
}

float SparkCell::RadarTarget::getBearing() const {
	const auto theta_1 = radians(mRadarLLA.Lat);
	const auto theta_2 = radians(mTgtLLA.Lat);
	const auto delta_1 = radians(mTgtLLA.Lat - mRadarLLA.Lat);
	const auto delta_2 = radians(mTgtLLA.Lon - mRadarLLA.Lon);

	const auto y = sin(delta_2) * cos(theta_2);
	const auto x = cos(theta_1) * sin(theta_2) - sin(theta_1) * cos(theta_2) * cos(delta_2);
	auto brng = atan2(y, x);

	brng = degrees(brng);
	brng = static_cast<int>(brng + 360) % 360;

	return brng;
}
float SparkCell::RadarTarget::getRange() const {
	const auto phi_1 = mRadarLLA.Lat * (std::numbers::pi / 180);
	const auto phi_2 = mTgtLLA.Lat * (std::numbers::pi / 180);
	const auto phi_delta = (mTgtLLA.Lat - mRadarLLA.Lat) * (std::numbers::pi / 180);
	const auto lambda_delta = (mTgtLLA.Lon - mRadarLLA.Lon) * (std::numbers::pi / 180);

	const auto a = (sin(phi_delta / 2) * sin(phi_delta / 2)) + (cos(phi_1) * cos(phi_2) * sin(lambda_delta / 2) * sin(lambda_delta / 2));
	const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

	const auto R = 3440.1;
	const float dist = R * c;

	return dist;
}

float SparkCell::RadarTarget::getHeading() const {
	return mTgtPBH.Heading;
}

float SparkCell::RadarTarget::getAA() const {
	return mTgtPBH.Heading - getBearing();
}

float SparkCell::RadarTarget::getATA() const {
	auto ata = static_cast<int>(getBearing() - mRadarPBH.Heading + 360) % 360;

	if (ata > 180)
		ata -=  360;

	return ata;
}
