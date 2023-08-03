#ifndef RADAR_H
#define RADAR_H

#include "RadarCursor.h"
#include "RadarTarget.h"
#include <functional>
#include <vector>

namespace SparkCell {

	class Radar
	{
	public:
		Radar(const Aircraft& host);

		void Update(const std::vector<RadarTarget>& targets);

		void SlewLeft();
		void SlewRight();
		void SlewUp();
		void SlewDown();

		void TryLock();

		const Aircraft& Host() const { return *mHost; }

		int GetAzimuth() const;
		int GetElevation() const;
		int GetBarSetting() const;
		int GetRange() const;

		float GetCursorAzimuth() const;
		float GetCursorRange() const;

		bool IsCursorNear(const RadarTarget& tgt) const;
		bool IsLockedTo(const RadarTarget& tgt) const;

		std::vector<RadarTarget> GetRadarTargets() const;
		const RadarTarget* const GetLockedTarget() const;

	private:

		int ScopeDown();
		int ScopeUp();

		std::vector<RadarTarget> mRadarTargets;

		const Aircraft* mHost;
		const RadarTarget* mLockedTarget = nullptr;

		int m_range = 40;

		int max_range = 160;
		int min_range = 10;

		int max_az = 60;

		RadarCursor crsr;
	};

}

#endif // RADAR_H
