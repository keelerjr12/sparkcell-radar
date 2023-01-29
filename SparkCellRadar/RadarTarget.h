#ifndef RADAR_TARGET_H
#define RADAR_TARGET_H

#include <Pdk.h>
#include <SimulatedRadarTypes.h>

namespace SparkCell {

	class RadarTarget
	{
	public:
		RadarTarget(const ::Radar::LLA& radarLLA, const ::Radar::PBH& radarPBH, const ::Radar::LLA& tgtLLA, const ::Radar::PBH& tgtPBH, const P3D::P3DDXYZ& tgtVel);

		float getAirspeed() const;
		float getBearing() const;
		float getRange() const;
		float getHeading() const;

		float getAA() const;
		float getATA() const;

	private:

		::Radar::LLA mRadarLLA;
		::Radar::PBH mRadarPBH;

		::Radar::LLA mTgtLLA;
		::Radar::PBH mTgtPBH;
		P3D::P3DDXYZ mTgtVel;
	};

}

#endif // RADAR_TARGET_H
