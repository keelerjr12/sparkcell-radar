#include "Radar.h"
#include <Pdk.h>
#include <PdkServices.h>
#include "Radar.h"
#include "RadarTarget.h"
#include "../p3d/RadarTypes.h"
#include "SimulatedRadarTypes.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <vector>
#include "Aircraft.h"

namespace SparkCell {

	static constexpr float DEFAULT_HORZ_SLEW_RATE = .6667f;
	static constexpr float DEFAULT_VERT_SLEW_RATE = .03056f;
	static constexpr int UPDATES_PER_SECOND = 18;

	void Radar::Update(const std::vector<RadarTarget>& targets) {
		mRadarTargets.clear();

		for (const auto& tgt : targets) {
			if (std::abs(tgt.ATA()) <= GetAzimuth()) {
				mRadarTargets.push_back(tgt);
			}
		}

		UpdateCursor();
	}

	void Radar::SlewLeft() {
		m_x_inc = DEFAULT_HORZ_SLEW_RATE;
	}

	void Radar::SlewRight() {
		m_x_inc = -DEFAULT_HORZ_SLEW_RATE;
	}

	void Radar::SlewUp() {
		m_y_inc = DEFAULT_VERT_SLEW_RATE;
	}

	void Radar::SlewDown() {
		m_y_inc = -DEFAULT_VERT_SLEW_RATE;
	}

	void Radar::TryLock()
	{
		for (const auto& tgt : this->mRadarTargets) {
			if (IsCursorNear(tgt)) {
				this->mLockedTarget = &tgt;
			} else {
				this->mLockedTarget = nullptr;
			}
		}
	}

	float Radar::GetCursorAzimuth() const {
		return m_cursor_az;
	}

	float Radar::GetCursorRange() const {
		return m_cursor_rng;
	}

	bool Radar::IsCursorNear(const RadarTarget& tgt) const {
		const auto NUM_DEGS = 3.0f;
		const auto NUM_MILES = 1.0f;

		const auto crs_bng = (static_cast<int>(std::round(mHost->heading() + m_cursor_az) + 360)) % 360;
		const auto ang_diff = std::abs(180.f - std::abs(std::abs(crs_bng - tgt.Bearing()) - 180.f));

		if (ang_diff < NUM_DEGS && abs(m_cursor_rng - tgt.Range()) < NUM_MILES) {
			return true;
		}

		return false;
	}

	bool Radar::IsLockedTo(const RadarTarget& tgt) const {
		const auto locked_tgt = GetLockedTarget();

		if (locked_tgt && tgt.Id() == locked_tgt->Id()) {
			return true;
		}

		return false;
	}

	int Radar::GetAzimuth() const {
		return 60;
	}

	int Radar::GetElevation() const {
		return 60;
	}

	int Radar::GetBarSetting() const {
		return 4;
	}

	int Radar::GetRange() const {
		return m_range;
	}

	std::vector<RadarTarget> Radar::GetRadarTargets() const {
		return mRadarTargets;
	}

	const RadarTarget* const Radar::GetLockedTarget() const {
		return mLockedTarget;
	}

	void Radar::UpdateCursor() {
		m_cursor_az -= m_x_inc * (60.0f / UPDATES_PER_SECOND);
		m_cursor_rng += GetRange() * m_y_inc;

		m_x_inc = 0;
		m_y_inc = 0;

		CheckBounds();
	}

	void Radar::CheckBounds() {
		m_cursor_az = min(m_cursor_az, GetAzimuth());
		m_cursor_az = max(m_cursor_az, -GetAzimuth());

		m_cursor_rng = min(m_cursor_rng, max_range);
		m_cursor_rng = max(m_cursor_rng, 0);

		const auto selected_rng = static_cast<float>(GetRange());
		const auto upper_rng_bound = selected_rng * .95;
		const auto lower_rng_bound = selected_rng * .05;

		if ((m_cursor_rng <= lower_rng_bound) && (GetRange() > min_range)) {
			m_range /= 2;
			m_cursor_rng = m_range / 2.f;
		} else if ((m_cursor_rng >= upper_rng_bound) && (GetRange() < max_range)) {
			m_range *= 2;
			m_cursor_rng = m_range / 2.f;
		}
	}

}
