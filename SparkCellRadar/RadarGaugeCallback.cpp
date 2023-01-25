#include "RadarGaugeCallback.h"
#include "RadarGaugeDrawable.h"
#include <fstream>
#include <iostream>

ULONG RadarGaugeCallback::AddRef()
{
	return 0;
}

ULONG RadarGaugeCallback::Release()
{
	return 0;
}

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	std::ofstream of("C:\\Users\\MITD09\\Documents\\out.txt");
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
