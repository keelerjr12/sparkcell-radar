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

		for (auto prop : gAircraftProperties)
		{
			if (SUCCEEDED(spUserObject->GetProperty(prop.szPropertyName, prop.units, val))) {
				std::cout << val << std::endl;
			}

			spUserObject->GetProperty(L"PLANE LATITUDE", L"Degrees", val);
			auto latitude = val;
			spUserObject->GetProperty(L"PLANE LONGITUDE", L"Degrees", val);
			auto longitude = val;
			spUserObject->GetProperty(L"PLANE ALTITUDE", L"Feet", val);
			auto altitude = val;

			P3D::P3DDXYZ latLongAlt = { latitude, longitude, altitude };
			UINT32 nObjects;
			UINT idArr[100];
			P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(latLongAlt, 120000, nObjects, idArr);

			std::cout << latLongAlt.dX << ", " << latLongAlt.dY << ", " << latLongAlt.dZ << std::endl;

			for (auto i = 0; i < 100; ++i)
				std::cout << idArr[i] << " ";
			std::cout << std::endl;
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
