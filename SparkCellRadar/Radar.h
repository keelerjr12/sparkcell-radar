#ifndef RADAR_H
#define RADAR_H

#include "RadarTarget.h"
#include <vector>

namespace SparkCell {


	class Radar
	{
	public:
		virtual void Update();

		virtual float GetAzimuth() const;
		virtual float GetRange() const;

		virtual std::vector<RadarTarget> GetRadarTargets() const;
		virtual const RadarTarget* const GetLockedTarget() const;

	private:
		std::vector<RadarTarget> mRadarTargets;

		RadarTarget* mLockedTarget = nullptr;
	};

}

#endif // RADAR_H
