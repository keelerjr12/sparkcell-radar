#include "RadarGaugeCallback.h"
#include <Pdk.h>
#include <PdkServices.h>
#include "RadarGaugeDrawable.h"
#include "RadarTypes.h"
#include <algorithm>
#include <fstream>
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

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	CComPtr<P3D::IBaseObjectV400> spUserObject;
	if (SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject))) {
		double val = 0.0;
		auto userId = spUserObject->GetId();

		for (auto prop : gAircraftProperties)
		{
			spUserObject->GetProperty(L"PLANE LATITUDE", L"Degrees", val);
			auto latitude = val;
			spUserObject->GetProperty(L"PLANE LONGITUDE", L"Degrees", val);
			auto longitude = val;
			spUserObject->GetProperty(L"PLANE ALTITUDE", L"Feet", val);
			auto altitude = val;

			P3D::P3DDXYZ latLongAlt = { latitude, longitude, altitude };
			std::cout << "USER OBJECT: \n";
			std::cout << userId << ", " << latLongAlt.dX << ", " << latLongAlt.dY << ", " << latLongAlt.dZ << std::endl;

			UINT32 nObjects = 100;
			UINT idArr[100];
			P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(latLongAlt, 6000*40, nObjects, idArr);

			for (auto i = 0; i < nObjects; ++i) {
				auto objId = idArr[i];
				CComPtr<P3D::IBaseObjectV400> aiObject;
				if (SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetObjectW(objId, &aiObject))) {
					aiObject->GetProperty(L"PLANE LATITUDE", L"Degrees", val);
					auto latitude = val;
					aiObject->GetProperty(L"PLANE LONGITUDE", L"Degrees", val);
					auto longitude = val;
					aiObject->GetProperty(L"PLANE ALTITUDE", L"Feet", val);
					auto altitude = val;

					P3D::P3DDXYZ latLongAlt = { latitude, longitude, altitude };
					std::cout << "OTHER OBJ: \n";
					std::cout << objId << ", " << latLongAlt.dX << ", " << latLongAlt.dY << ", " << latLongAlt.dZ << std::endl;
				}
			}
		}
	}
}

bool RadarGaugeCallback::GetPropertyValue(SINT32 id, FLOAT64* pValue)
{
	return false;
}

bool RadarGaugeCallback::GetPropertyValue(SINT32 id, LPCSTR* pszValue)
{
	return false;
}

bool RadarGaugeCallback::GetPropertyValue(SINT32 id, LPCWSTR* pszValue)
{
	return false;
}

bool RadarGaugeCallback::SetPropertyValue(SINT32 id, FLOAT64 value)
{
	return false;
}

bool RadarGaugeCallback::SetPropertyValue(SINT32 id, LPCSTR szValue)
{
	return false;
}

bool RadarGaugeCallback::SetPropertyValue(SINT32 id, LPCWSTR szValue)
{
	return false;
}

IGaugeCDrawable* RadarGaugeCallback::CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters)
{
	RadarGaugeDrawable* radarGaugeDrawable = nullptr;

	if (id == RadarGaugeDrawableId::RADAR) {
		radarGaugeDrawable = new RadarGaugeDrawable(pParameters);
	}

	return radarGaugeDrawable;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarGaugeCallback);
