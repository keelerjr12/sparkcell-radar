#include "RadarAircraftCallback.h"
#include "RadarGaugeCallback.h"
#include <Pdk.h>
#include <PdkServices.h>

RadarAircraftCallback::RadarAircraftCallback(UINT32 containerId) : m_RefCount(1), mContainerId(containerId) {

}

IAircraftCCallback* RadarAircraftCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

IGaugeCCallback* RadarAircraftCallback::CreateGaugeCCallback()
{
	return new RadarGaugeCallback(mContainerId, this->user_aircraft);
}

void RadarAircraftCallback::Update()
{
	CComPtr<P3D::IBaseObjectV400> spUserObject;

	if (SUCCEEDED(P3D::PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject))) {
		CComPtr<P3D::IBaseObjectV400> spUserAvatar;
		P3D::PdkServices::GetSimObjectManager()->GetUserAvatar(&spUserAvatar);

		auto userObjectId = spUserObject->GetId();
		auto userAvatarId = spUserAvatar->GetId();

		this->user_aircraft.Update(spUserObject);
	}
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarAircraftCallback);
