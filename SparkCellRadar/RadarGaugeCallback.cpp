#include "RadarGaugeCallback.h"
#include "Aircraft.h"
#include "gauges.h"
#include "Radar.h"
#include "RadarGaugeDrawable.h"
#include "Pdk.h"

#include <debugapi.h>

float GetAxisPercent(int sign, UINT32 evdata)
{
	if (evdata & 0xFFFF0000)
	{
		sign *= -1;
		evdata = 0xFFFFFFFF - evdata;
	}
	evdata = evdata & 0x0000FFFF;
	float axisPercent = (evdata / 65535.0f) * sign;

	return axisPercent;
}

void FSAPI KeyEventHandler(ID32 event, UINT32 evdata, PVOID userdata)
{
	auto radar = static_cast<SparkCell::Radar*>(userdata);


	switch (event)
	{
	case KEY_HOTAS_KEY_A0:
		std::cout << "Lock!\n";
		break;
	case KEY_GUN_SYSTEM_ROTATE_GUNS:
		if (evdata == 45) {
			radar->SlewUp(.5f);
			radar->SlewRight(-.5f);
		}
		else if (evdata == 90)
		{
			radar->SlewRight(-.5f);
		}
		else if (evdata == 135) {
			radar->SlewDown(-.5f);
			radar->SlewRight(-.5f);
		}
		else if (evdata == 180)
		{
			radar->SlewDown(-.5f);
		}
		else if (evdata == 225)
		{
			radar->SlewDown(-.5f);
			radar->SlewLeft(.5f);
		}
		else if (evdata == 270)
		{
			radar->SlewLeft(.5f);
		}
		else if (evdata == 315)
		{
			radar->SlewUp(.5f);
			radar->SlewLeft(.5f);
		}
		else if (evdata == 0)
		{
			radar->SlewUp(.5f);
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
