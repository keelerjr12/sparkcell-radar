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

		float Airspeed() const;
		float Altitude() const;

		float Bearing() const;
		float Range() const;
		float Heading() const;

		float AA() const;
		float ATA() const;

	private:

		// TODO: pointers?
		Aircraft host_;
		Aircraft target_;
	};

}

#endif // RADAR_TARGET_H
