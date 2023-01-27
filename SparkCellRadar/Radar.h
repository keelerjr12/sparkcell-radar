#ifndef RADAR_H
#define RADAR_H

#include <vector>

namespace SparkCell {

	struct RadarHit {
		float bearing;
		float range;
	};

	class Radar
	{
	public:
		virtual void Update();

		virtual float GetAzimuth() const;
		virtual float GetRange() const;

		virtual std::vector<RadarHit> GetRadarHits() const;

	private:
		std::vector<RadarHit> mRadarHits;
	};

}

#endif // RADAR_H
