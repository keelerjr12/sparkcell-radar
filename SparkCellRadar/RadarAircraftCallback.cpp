#include "RadarAircraftCallback.h"
#include "RadarGaugeCallback.h"

IAircraftCCallback* RadarAircraftCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

IGaugeCCallback* RadarAircraftCallback::CreateGaugeCCallback()
{
	return new RadarGaugeCallback(mContainerId);
}

void RadarAircraftCallback::Update()
{
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarAircraftCallback);
