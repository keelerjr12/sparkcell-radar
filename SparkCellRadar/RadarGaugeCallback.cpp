#include "RadarGaugeCallback.h"
#include "Aircraft.h"
#include "gauges.h"
#include "Radar.h"
#include "RadarGaugeDrawable.h"

RadarGaugeCallback::RadarGaugeCallback(UINT32 containerId, const SparkCell::Aircraft& host) : m_RefCount(1), mContainerId(containerId), mRadar(new SparkCell::Radar(host)) { } 

RadarGaugeCallback::~RadarGaugeCallback() {
	delete mRadar;
}

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	mRadar->SlewRight();
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
	RadarGaugeDrawable* radarGaugeDrawable = nullptr;

	if (id == RadarGaugeDrawableId::RADAR) {
		radarGaugeDrawable = new RadarGaugeDrawable(pParameters, mRadar);
	}

	return radarGaugeDrawable;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarGaugeCallback);
