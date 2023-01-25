#include "RadarPanelCallback.h"
#include "RadarAircraftCallback.h"

ULONG RadarPanelCallback::AddRef()
{
	return 0;
}

ULONG RadarPanelCallback::Release()
{
	return 0;
}

IPanelCCallback* RadarPanelCallback::QueryInterface(LPCSTR pszInterface)
{
	return nullptr;
}

UINT32 RadarPanelCallback::GetVersion()
{
	return UINT32();
}

IAircraftCCallback* RadarPanelCallback::CreateAircraftCCallback(UINT32 ContainerID)
{
	return new RadarAircraftCallback();
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
