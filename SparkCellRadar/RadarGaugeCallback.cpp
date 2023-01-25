#include "RadarGaugeCallback.h"
#include "RadarGaugeDrawable.h"
#include <fstream>
#include <iostream>

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	std::ofstream of("C:\\Users\\ADMIN\\Documents\\out.txt");
	of << "UPDATE\n";
	std::cout << "UPDATE\n";
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
