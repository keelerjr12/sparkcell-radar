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
#include <iostream>
#include <vector>
#include "Aircraft.h"

// A list of simulation variables / registered properties to be collected.
/*static std::vector<AircraftPropertyTable> gAircraftProperties =
{
    { L"SIM TIME",                      L"Seconds",                 },
    { L"PLANE LATITUDE",                L"Degrees"                  },
    { L"PLANE LONGITUDE",               L"Degrees"                  },
    { L"PLANE ALTITUDE",                L"Feet"                     },
    { L"PLANE PITCH DEGREES",           L"Degrees"                  },
    { L"PLANE BANK DEGREES",            L"Degrees"                  },
    { L"PLANE HEADING DEGREES TRUE",    L"Degrees"                  },
    { L"VELOCITY WORLD X",              L"Feet per second"          },
    { L"VELOCITY WORLD Y",              L"Feet per second"          },
    { L"VELOCITY WORLD Z",              L"Feet per second"          }
};*/
namespace SparkCell {

	static constexpr float DEFAULT_HORZ_SLEW_RATE = .5f;
	static constexpr float DEFAULT_VERT_SLEW_RATE = .5f;

	void Radar::Update(const std::vector<RadarTarget>& targets) {

		/*CComPtr<P3D::IBaseObjectV400> spUserAvatar;
		P3D::PdkServices::GetSimObjectManager()->GetUserAvatar(&spUserAvatar);

		auto userAvatarId = spUserAvatar->GetId();
		auto userObjectId = mHost->id();

		UINT32 nObjects = 100;
		UINT idArr[100];
		P3D::P3DDXYZ dxyz{ mHost->lat(), mHost->lon(), mHost->alt()};
		P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(dxyz, 6000*40, nObjects, idArr);

		for (auto i = 0; i < nObjects; ++i) {

			auto objId = idArr[i];
			CComPtr<P3D::IBaseObjectV400> aiObject;

			if ((objId != userObjectId) && (objId != userAvatarId) && SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetObjectW(objId, &aiObject))) {
				const auto targetAircraft = Aircraft(aiObject);
				const auto tgt = RadarTarget(*mHost, targetAircraft);

				if (abs(tgt.ATA()) <= GetAzimuth()) {
					mRadarTargets.push_back(tgt);
				}
			}
		}*/
		mRadarTargets.clear();

		for (const auto& tgt : targets) {
			if (std::abs(tgt.ATA()) <= GetAzimuth()) {
				mRadarTargets.push_back(tgt);
			}
		}

		m_cursor_az -= m_x_inc * 1.f / 18 * 60;
		m_cursor_rng += m_y_inc * 1.f / 18 * 60;
		m_x_inc = 0;
		m_y_inc = 0;
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

	int Radar::GetCursorAzimuth() const {
		return static_cast<int>(m_cursor_az);
	}

	int Radar::GetCursorRange() const {
		return static_cast<int>(m_cursor_rng);
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
		return 40;
	}

	std::vector<RadarTarget> SparkCell::Radar::GetRadarTargets() const {
		return mRadarTargets;
	}

	const RadarTarget* const SparkCell::Radar::GetLockedTarget() const {
		return mLockedTarget;
	}

}
