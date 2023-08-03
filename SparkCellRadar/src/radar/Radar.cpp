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


	Radar::Radar(const Aircraft& host) : 
		mHost(&host), 
		crsr(m_range, max_range, min_range, max_az, std::bind(&Radar::ScopeUp, this), std::bind(&Radar::ScopeDown, this)) { }

	void Radar::Update(const std::vector<RadarTarget>& targets) {
		mRadarTargets.clear();

		for (const auto& tgt : targets) {
			if (std::abs(tgt.ATA()) <= GetAzimuth()) {
				mRadarTargets.push_back(tgt);
			}
		}

		crsr.Update();
	}

	void Radar::SlewLeft() {
		crsr.SlewLeft();
	}

	void Radar::SlewRight() {
		crsr.SlewRight();
	}

	void Radar::SlewUp() {
		crsr.SlewUp();
	}

	void Radar::SlewDown() {
		crsr.SlewDown();
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
		return crsr.Azimuth();
	}

	float Radar::GetCursorRange() const {
		return crsr.Range();
	}

	bool Radar::IsCursorNear(const RadarTarget& tgt) const {
		const auto crs_bng = (static_cast<int>(std::round(mHost->heading() + crsr.Azimuth()) + 360)) % 360;
		const auto ang_diff = std::abs(180.f - std::abs(std::abs(crs_bng - tgt.Bearing()) - 180.f));

		if (crsr.IsNear(ang_diff, tgt.Range())) {
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

	int Radar::ScopeDown() {
		m_range /= 2;
		return m_range;
	}
	
	int Radar::ScopeUp() {
		m_range *= 2;
		return m_range;
	}

}
