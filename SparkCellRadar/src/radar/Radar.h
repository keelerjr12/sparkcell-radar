#ifndef RADAR_H
#define RADAR_H

#include "RadarTarget.h"
#include <vector>

namespace SparkCell {

	class Radar
	{
	public:
		Radar(const Aircraft& host) : mHost(&host) { }

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
		void UpdateCursor();
		void CheckBounds();

		std::vector<RadarTarget> mRadarTargets;

		const Aircraft* mHost;
		const RadarTarget* mLockedTarget = nullptr;

		int m_range = 40;

		float m_cursor_az = 0;
		float m_cursor_rng = 20;

		float m_x_inc = 0.f;
		float m_y_inc = 0.f;

		int max_range = 160;
		int min_range = 10;
	};

}

#endif // RADAR_H
