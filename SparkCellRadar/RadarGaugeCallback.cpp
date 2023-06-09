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
		case KEY_HOTAS_SLEW_AXIS_X:
		{
			auto ap1 = GetAxisPercent(1, evdata);
			auto ap2 = GetAxisPercent(-1, evdata);

			const auto a = std::to_wstring(ap1);
			const auto b = std::to_wstring(ap2);
			const auto c = a + L" " + b + L"\n";
			OutputDebugString(c.c_str());

			if (ap1 > 0)
				radar->SlewRight(ap1);
			else
				radar->SlewLeft(ap1);

			break;
		}
		case KEY_HOTAS_SLEW_AXIS_Y:
		{
			auto ap1 = GetAxisPercent(1, evdata);

			if (ap1 > 0)
				radar->SlewDown(ap1);
			else
				radar->SlewUp(ap1);

			break;
		}
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
