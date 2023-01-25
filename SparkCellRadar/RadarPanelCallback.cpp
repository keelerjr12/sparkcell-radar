#include "RadarPanelCallback.h"
#include "RadarAircraftCallback.h"

IPanelCCallback* RadarPanelCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

UINT32 RadarPanelCallback::GetVersion()
{
	//return UINT32();
	return 1;
}

IAircraftCCallback* RadarPanelCallback::CreateAircraftCCallback(UINT32 containerId)
{
	return new RadarAircraftCallback(containerId);
}

bool RadarPanelCallback::ConvertStringToProperty(LPCSTR keyword, SINT32* pID)
{
	return false;
}

bool RadarPanelCallback::ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword)
{
	return false;
}

bool RadarPanelCallback::GetPropertyUnits(SINT32 id, ENUM* pEnum)
{
	return false;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarPanelCallback);