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

		void SlewLeft(float inc);
		void SlewRight(float inc);
		void SlewUp(float inc);
		void SlewDown(float inc);

		void TryLock();

		const Aircraft& Host() const { return *mHost; }

		int GetAzimuth() const;
		int GetElevation() const;
		int GetBarSetting() const;
		int GetRange() const;

		int GetCursorAzimuth() const;
		int GetCursorRange() const;

		bool IsCursorNear(const RadarTarget& tgt) const;

		std::vector<RadarTarget> GetRadarTargets() const;
		const RadarTarget* const GetLockedTarget() const;

	private:
		std::vector<RadarTarget> mRadarTargets;

		const Aircraft* mHost;
		const RadarTarget* mLockedTarget = nullptr;

		float m_cursor_az = 0;
		float m_cursor_rng = 20;

		float m_x_inc = 0.f;
		float m_y_inc = 0.f;
	};

}

#endif // RADAR_H