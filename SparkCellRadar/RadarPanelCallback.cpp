#include "RadarPanelCallback.h"
#include "RadarAircraftCallback.h"

// Note: The items in the property table correspond to the indices that
// will be returned in the Get/Set Property functions
struct PROPERTY_TABLE
{
    PCSTRINGZ szPropertyName;
    PCSTRINGZ szUnitsName;
    ENUM units;
};

static PROPERTY_TABLE NAVIGATION_PROPERTY_TABLE[] =
{
    { "Nav",                        "Number",       UNITS_UNKNOWN },
};

const PROPERTY_TABLE* GetPropertyTable(UINT& uLength)
{
	uLength = LENGTHOF(NAVIGATION_PROPERTY_TABLE);
	return NAVIGATION_PROPERTY_TABLE;
}

RadarPanelCallback::RadarPanelCallback() : m_RefCount(1) { 
	// init property table
	for (int n = 0; n < 5; n++)
	{
		if (ImportTable.PANELSentry.fnptr != NULL &&
			NAVIGATION_PROPERTY_TABLE[n].units == UNITS_UNKNOWN)
		{
			NAVIGATION_PROPERTY_TABLE[n].units = get_units_enum(NAVIGATION_PROPERTY_TABLE[n].szUnitsName);
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
    const PROPERTY_TABLE* parPropertyTable = GetPropertyTable(uNumProperties);

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
    const PROPERTY_TABLE* parPropertyTable = GetPropertyTable(uNumProperties);

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