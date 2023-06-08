#include "Radar.h"
#include <Pdk.h>
#include <PdkServices.h>
#include "Radar.h"
#include "RadarTarget.h"
#include "RadarTypes.h"
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

void SparkCell::Radar::Update() {
	mRadarTargets.clear();
	mLockedTarget = nullptr;

	CComPtr<P3D::IBaseObjectV400> spUserAvatar;
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

			if (abs(tgt.getATA()) <= GetAzimuth()) {
				mRadarTargets.push_back(tgt);
				mLockedTarget = &mRadarTargets.back();
			}
		}
	}
}

void SparkCell::Radar::SlewRight() {
	m_cursor_az = m_cursor_az + static_cast<int>(1.f / 18 * 20);
}

int SparkCell::Radar::GetCursorAzimuth() const {
	return m_cursor_az;
}

int SparkCell::Radar::GetAzimuth() const {
	return 60;
}

int SparkCell::Radar::GetElevation() const {
	return 60;
}

int SparkCell::Radar::GetRange() const {
	return 40;
}

std::vector<SparkCell::RadarTarget> SparkCell::Radar::GetRadarTargets() const {
	return mRadarTargets;
}

const SparkCell::RadarTarget* const SparkCell::Radar::GetLockedTarget() const {
	return mLockedTarget;
}
