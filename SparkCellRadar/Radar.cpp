#include "Radar.h"
#include <Pdk.h>
#include <PdkServices.h>
#include "Radar.h"
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

float calculateDistance(const Radar::LLA& lla_1, const Radar::LLA& lla_2) {
	const auto phi_1 = lla_1.Lat * (std::numbers::pi / 180);
	const auto phi_2 = lla_2.Lat * (std::numbers::pi / 180);
	const auto phi_delta = (lla_2.Lat - lla_1.Lat) * (std::numbers::pi / 180);
	const auto lambda_delta = (lla_2.Lon - lla_1.Lon) * (std::numbers::pi / 180);

	const auto a = (sin(phi_delta / 2) * sin(phi_delta / 2)) + (cos(phi_1) * cos(phi_2) * sin(lambda_delta / 2) * sin(lambda_delta / 2));
	const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

	const auto R = 3440.1;
	const float dist = R * c;

	return dist;
}

float radians(float degs) {
	return degs * std::numbers::pi / 180;
}

float degrees(float rads) {
	return rads * 180 / std::numbers::pi;
}

float calculateBearing(const Radar::LLA& lla_1, const Radar::LLA& lla_2) {
	const auto theta_1 = radians(lla_1.Lat);
	const auto theta_2 = radians(lla_2.Lat);
	const auto delta_1 = radians(lla_2.Lat - lla_1.Lat);
	const auto delta_2 = radians(lla_2.Lon - lla_1.Lon);

	const auto y = sin(delta_2) * cos(theta_2);
	const auto x = cos(theta_1) * sin(theta_2) - sin(theta_1) * cos(theta_2) * cos(delta_2);
	auto brng = atan2(y, x);

	brng = degrees(brng);
	brng = static_cast<int>(brng + 360) % 360;

	return brng;
}

float calculateAON(const Radar::LLA& userLLA, const Radar::PBH& userPBH, const Radar::LLA& rhLLA) {
	auto aon = static_cast<int>(calculateBearing(userLLA, rhLLA) - userPBH.Heading + 360) % 360;

	if (aon > 180)
		aon = aon - 360;

	return aon;
}

void SparkCell::Radar::Update() {
	mRadarHits.clear();

	CComPtr<P3D::IBaseObjectV400> spUserObject;
	if (SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject))) {
		auto userId = spUserObject->GetId();
		auto userTelemetry = getTelemetryData(*spUserObject);
		auto userPBH = getPBH(*spUserObject);

		std::cout << userId << ", " << userTelemetry.Lat << ", " << userTelemetry.Lon << ", " << userTelemetry.Alt << ", " << userPBH.Heading << std::endl;

		UINT32 nObjects = 100;
		UINT idArr[100];
		P3D::P3DDXYZ dxyz{ userTelemetry.Lat, userTelemetry.Lon, userTelemetry.Alt };
		P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(dxyz, 6000*40, nObjects, idArr);

		for (auto i = 0; i < nObjects; ++i) {
			auto objId = idArr[i];
			CComPtr<P3D::IBaseObjectV400> aiObject;
			if ((objId != userId) && SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetObjectW(objId, &aiObject))) {
				std::cout << "OTHER OBJ: \n";
				auto radarHitTelemetry = getTelemetryData(*aiObject);
				std::cout << objId << ", " << radarHitTelemetry.Lat << ", " << radarHitTelemetry.Lon << ", " << radarHitTelemetry.Alt << std::endl;

				const auto brng = calculateBearing(userTelemetry, radarHitTelemetry);
				const auto aon = calculateAON(userTelemetry, userPBH, radarHitTelemetry);
				const auto dist = calculateDistance(userTelemetry, radarHitTelemetry);

				std::cout << "brng: " << brng << ", aon: " << aon << "dist: " << dist << std::endl;
				mRadarHits.push_back({aon, dist});
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

std::vector<SparkCell::RadarHit> SparkCell::Radar::GetRadarHits() const {
	return mRadarHits;
}