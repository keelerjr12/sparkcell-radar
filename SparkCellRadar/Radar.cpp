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

// A list of simulation variables / registered properties to be collected.
static std::vector<AircraftPropertyTable> gAircraftProperties =
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
};

Radar::LLA getTelemetryData(const P3D::IBaseObjectV400& obj) {
	auto latitude = 0.0;
	obj.GetProperty(L"PLANE LATITUDE", L"Degrees", latitude);
	auto longitude = 0.0;
	obj.GetProperty(L"PLANE LONGITUDE", L"Degrees", longitude);
	auto altitude = 0.0;
	obj.GetProperty(L"PLANE ALTITUDE", L"Feet", altitude);

	return Radar::LLA(latitude, longitude, altitude);
}

Radar::PBH getPBH(const P3D::IBaseObjectV400& obj) {
	auto pitch = 0.0;
	obj.GetProperty(L"PLANE PITCH DEGREES", L"Degrees", pitch);
	auto bank = 0.0;
	obj.GetProperty(L"PLANE BANK DEGREES", L"Degrees", bank);
	auto heading = 0.0;
	obj.GetProperty(L"PLANE HEADING DEGREES TRUE", L"Degrees", heading);

	return Radar::PBH{ pitch, bank, heading };
}

P3D::P3DDXYZ getVelocities(const P3D::IBaseObjectV400& obj) {
	auto vx = 0.0;
	obj.GetProperty(L"VELOCITY WORLD X", L"Knots", vx);
	auto vy = 0.0;
	obj.GetProperty(L"VELOCITY WORLD Y", L"Knots", vy);
	auto vz = 0.0;
	obj.GetProperty(L"VELOCITY WORLD Z", L"Knots", vz);

	return P3D::P3DDXYZ { vx, vy, vz };
}

float calculateCATA(const SparkCell::RadarTarget& tgt, const P3D::P3DDXYZ A_VelVec, const Radar::PBH& B_PBH, const P3D::P3DDXYZ B_VelVec) {
	/*const auto bearing = tgt.getBearing();
	const auto vec = sin(radians(bearing - B_PBH.Heading));

	const auto A_vel = sqrt(A_VelVec.dX * A_VelVec.dX + A_VelVec.dY * A_VelVec.dY);
	const auto B_vel = sqrt(B_VelVec.dX * B_VelVec.dX + B_VelVec.dY * B_VelVec.dY);

	const auto velRatio = A_vel / B_vel;

	const auto cataRad = asin(velRatio * vec);
	const auto cataDeg = degrees(cataRad);

	return cataDeg;*/
	return 0.f;
}

void SparkCell::Radar::Update() {
	mRadarTargets.clear();

	CComPtr<P3D::IBaseObjectV400> spUserObject;
	if (SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject))) {
		CComPtr<P3D::IBaseObjectV400> spUserAvatar;
		P3D::PdkServices::GetSimObjectManager()->GetUserAvatar(&spUserAvatar);

		auto userObjectId = spUserObject->GetId();
		auto userAvatarId = spUserAvatar->GetId();

		auto userTelemetry = getTelemetryData(*spUserObject);
		auto userPBH = getPBH(*spUserObject);
		const auto userVelocities = getVelocities(*spUserObject);

		UINT32 nObjects = 100;
		UINT idArr[100];
		P3D::P3DDXYZ dxyz{ userTelemetry.Lat, userTelemetry.Lon, userTelemetry.Alt };
		P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(dxyz, 6000*40, nObjects, idArr);

		for (auto i = 0; i < nObjects; ++i) {

			auto objId = idArr[i];
			CComPtr<P3D::IBaseObjectV400> aiObject;

			if ((objId != userObjectId) && (objId != userAvatarId) && SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetObjectW(objId, &aiObject))) {
				auto radarHitTelemetry = getTelemetryData(*aiObject);
				auto radarHitPBH = getPBH(*aiObject);
				const auto radarHitVelocities = getVelocities(*aiObject);

				const auto tgt = RadarTarget(userTelemetry, userPBH, radarHitTelemetry, radarHitPBH, radarHitVelocities);

				mRadarTargets.push_back(tgt);
				mLockedTarget = &mRadarTargets.back();
			}
		}
	}
}

float SparkCell::Radar::GetAzimuth() const {
	return 60.f;
}

float SparkCell::Radar::GetRange() const {
	return 40.f;
}

std::vector<SparkCell::RadarTarget> SparkCell::Radar::GetRadarTargets() const {
	return mRadarTargets;
}

const SparkCell::RadarTarget* const SparkCell::Radar::GetLockedTarget() const {
	return mLockedTarget;
}
