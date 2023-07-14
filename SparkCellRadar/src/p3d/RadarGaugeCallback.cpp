#include "RadarGaugeCallback.h"
#include "RadarGaugeDrawable.h"
#include "../radar/Aircraft.h"
#include "../radar/Radar.h"
#include "gauges.h"
#include "Pdk.h"

#include <debugapi.h>

void FSAPI KeyEventHandler(ID32 event, UINT32 evdata, PVOID userdata)
{
	auto radar = static_cast<SparkCell::Radar*>(userdata);

	switch (event)
	{
	case KEY_HOTAS_KEY_A0:
		radar->TryLock();
		break;
	case KEY_GUN_SYSTEM_ROTATE_GUNS:
		if (evdata == 45) {
			radar->SlewUp();
			radar->SlewRight();
		}
		else if (evdata == 90)
		{
			radar->SlewRight();
		}
		else if (evdata == 135) {
			radar->SlewDown();
			radar->SlewRight();
		}
		else if (evdata == 180)
		{
			radar->SlewDown();
		}
		else if (evdata == 225)
		{
			radar->SlewDown();
			radar->SlewLeft();
		}
		else if (evdata == 270)
		{
			radar->SlewLeft();
		}
		else if (evdata == 315)
		{
			radar->SlewUp();
			radar->SlewLeft();
		}
		else if (evdata == 0)
		{
			radar->SlewUp();
		}
		break;
    };
}
RadarGaugeCallback::RadarGaugeCallback(UINT32 containerId, const SparkCell::Aircraft& host) : m_RefCount(1), mContainerId(containerId), mRadar(new SparkCell::Radar(host)) {
    P3D::PdkServices::GetPanelSystem()->RegisterKeyEventHandler(static_cast<GAUGE_KEY_EVENT_HANDLER>(KeyEventHandler), mRadar);
}

RadarGaugeCallback::~RadarGaugeCallback() {
    P3D::PdkServices::GetPanelSystem()->UnregisterKeyEventHandler(static_cast<GAUGE_KEY_EVENT_HANDLER>(KeyEventHandler), mRadar);
	delete mRadar;
}

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	mRadar->Update();
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
	if (id == RadarGaugeDrawableId::RADAR) {
		return new RadarGaugeDrawable(pParameters, mRadar);
	}

	return nullptr;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarGaugeCallback);
