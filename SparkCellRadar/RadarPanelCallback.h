#ifndef RADAR_PANEL_CALLBACK_H
#define RADAR_PANEL_CALLBACK_H

#include <gauges.h>

class RadarPanelCallback : public IPanelCCallback {
public:
	// Inherited via IPanelCCallback
	virtual ULONG AddRef() override;

	virtual ULONG Release() override;

	virtual IPanelCCallback* QueryInterface(LPCSTR pszInterface) override;

	virtual UINT32 GetVersion() override;

	virtual IAircraftCCallback* CreateAircraftCCallback(UINT32 ContainerID) override;

	virtual bool ConvertStringToProperty(LPCSTR keyword, SINT32* pID) override;

	virtual bool ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword) override;

	virtual bool GetPropertyUnits(SINT32 id, ENUM* pEnum) override;
};

#endif // RADAR_PANEL_CALLBACK_H
