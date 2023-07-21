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
RadarGaugeCallback::RadarGaugeCallback(UINT32 containerId, const SparkCell::Aircraft& host) : m_RefCount(1), mContainerId(containerId), radar_(new SparkCell::Radar(host)) {
    P3D::PdkServices::GetPanelSystem()->RegisterKeyEventHandler(static_cast<GAUGE_KEY_EVENT_HANDLER>(KeyEventHandler), radar_);
}

RadarGaugeCallback::~RadarGaugeCallback() {
    P3D::PdkServices::GetPanelSystem()->UnregisterKeyEventHandler(static_cast<GAUGE_KEY_EVENT_HANDLER>(KeyEventHandler), radar_);
	delete radar_;
}

IGaugeCCallback* RadarGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

void RadarGaugeCallback::Update()
{
	CComPtr<P3D::IBaseObjectV400> spUserAvatar;
	P3D::PdkServices::GetSimObjectManager()->GetUserAvatar(&spUserAvatar);

	const auto& host = radar_->Host();

	const auto userAvatarId = spUserAvatar->GetId();
	const auto userObjectId = host.id();

	UINT32 nObjects = 100;
	UINT idArr[100];
	P3D::P3DDXYZ dxyz{ host.lat(), host.lon(), host.alt()};
	P3D::PdkServices::GetSimObjectManager()->GetObjectsInRadius(dxyz, 6000*40, nObjects, idArr);

	std::vector<SparkCell::RadarTarget> tgts;
	for (auto i = 0; i < nObjects; ++i) {

		auto objId = idArr[i];
		CComPtr<P3D::IBaseObjectV400> aiObject;

		if ((objId != userObjectId) && (objId != userAvatarId) && SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetObjectW(objId, &aiObject))) {
			const auto targetAircraft = SparkCell::Aircraft(aiObject);
			const auto tgt = SparkCell::RadarTarget(host, targetAircraft);
			tgts.push_back(tgt);
		}
	}

	radar_->Update(tgts);
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
		return new RadarGaugeDrawable(pParameters, radar_);
	}

	return nullptr;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarGaugeCallback);
