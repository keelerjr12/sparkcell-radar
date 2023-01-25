#include "RadarAircraftCallback.h"
#include "RadarGaugeCallback.h"

ULONG RadarAircraftCallback::AddRef()
{
	return 0;
}

ULONG RadarAircraftCallback::Release()
{
	return 0;
}

IAircraftCCallback* RadarAircraftCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

IGaugeCCallback* RadarAircraftCallback::CreateGaugeCCallback()
{
	return new RadarGaugeCallback();
}

void RadarAircraftCallback::Update()
{
}
