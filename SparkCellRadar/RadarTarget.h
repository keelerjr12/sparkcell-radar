#ifndef RADAR_TARGET_H
#define RADAR_TARGET_H

#include <Pdk.h>
#include <SimulatedRadarTypes.h>
#include "Aircraft.h"

namespace SparkCell {

	class RadarTarget
	{
	public:
		RadarTarget(const Aircraft& host, const Aircraft& tgt);

		float getAirspeed() const { return mTarget.airspeed(); }
		float getAltitude() const { return mTarget.alt(); }

		float getBearing() const;
		float getRange() const;
		float getHeading() const;

		float getAA() const;
		float getATA() const;

	private:

		Aircraft mHost;
		Aircraft mTarget;
	};

}

#endif // RADAR_TARGET_H
