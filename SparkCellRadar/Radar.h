#ifndef RADAR_H
#define RADAR_H

#include "RadarTarget.h"
#include <vector>

namespace SparkCell {

	class Radar
	{
	public:
		Radar(const Aircraft& host) : mHost(&host) { }

		virtual void Update();

		void SlewRight();

		const Aircraft& Host() const { return *mHost; }

		virtual int GetAzimuth() const;
		virtual int GetElevation() const;
		virtual int GetRange() const;

		int GetCursorAzimuth() const;

		virtual std::vector<RadarTarget> GetRadarTargets() const;
		virtual const RadarTarget* const GetLockedTarget() const;

	private:
		std::vector<RadarTarget> mRadarTargets;

		const Aircraft* mHost;
		RadarTarget* mLockedTarget = nullptr;

		int m_cursor_az = -60;
	};

}

#endif // RADAR_H
