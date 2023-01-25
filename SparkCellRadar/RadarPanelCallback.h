#ifndef RADAR_PANEL_CALLBACK_H
#define RADAR_PANEL_CALLBACK_H

#include <gauges.h>

class RadarPanelCallback : public IPanelCCallback {
public:
	RadarPanelCallback() : m_RefCount(1) { }

	virtual IPanelCCallback* QueryInterface(LPCSTR pszInterface) override;

	virtual UINT32 GetVersion() override;

	virtual IAircraftCCallback* CreateAircraftCCallback(UINT32 containerId) override;

	virtual bool ConvertStringToProperty(LPCSTR keyword, SINT32* pID) override;

	virtual bool ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword) override;

	virtual bool GetPropertyUnits(SINT32 id, ENUM* pEnum) override;

private:

	DECLARE_PANEL_CALLBACK_REFCOUNT(RadarPanelCallback);
};

#endif // RADAR_PANEL_CALLBACK_H
