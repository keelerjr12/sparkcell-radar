#include "RadarPanelCallback.h"
#include "RadarAircraftCallback.h"
#include "RadarTypes.h"

static PropertyTable RADAR_PROPERTY_TABLE[] =
{
    { "Radar",  "Number",   UNITS_UNKNOWN },
};

const PropertyTable* GetPropertyTable(UINT& uLength)
{
	uLength = LENGTHOF(RADAR_PROPERTY_TABLE);
	return RADAR_PROPERTY_TABLE;
}

RadarPanelCallback::RadarPanelCallback() : m_RefCount(1) { 
	// init property table
	for (int n = 0; n < 1; n++)
	{
		if (ImportTable.PANELSentry.fnptr != NULL &&
			RADAR_PROPERTY_TABLE[n].units == UNITS_UNKNOWN)
		{
			RADAR_PROPERTY_TABLE[n].units = get_units_enum(RADAR_PROPERTY_TABLE[n].szUnitsName);
		}
	}
}

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
    if (!keyword)
    {
        return false;
    }
    if (!pID)
    {
        return false;
    }

    UINT uNumProperties;
    const PropertyTable* parPropertyTable = GetPropertyTable(uNumProperties);

    for (UINT i = 0; i < uNumProperties; i++)
    {
        if (_stricmp(parPropertyTable[i].szPropertyName, keyword) == 0)
        {
            *pID = i;
            return true;
        }
    }
    return false;
}

bool RadarPanelCallback::ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword)
{
    if (!pKeyword)
    {
        return false;
    }

    UINT uNumProperties;
    const PropertyTable* parPropertyTable = GetPropertyTable(uNumProperties);

    if (id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }
    *pKeyword = parPropertyTable[id].szPropertyName;

    return true;

}

bool RadarPanelCallback::GetPropertyUnits(SINT32 id, ENUM* pEnum)
{
	return false;
}

DEFINE_PANEL_CALLBACK_REFCOUNT(RadarPanelCallback);