#ifndef RADAR_AIRCRAFT_CALLBACK_H
#define RADAR_AIRCRAFT_CALLBACK_H

#include <gauges.h>

class RadarAircraftCallback : public IAircraftCCallback {
public:
	RadarAircraftCallback(UINT32 containerId) : m_RefCount(1), mContainerId(containerId) { }

	virtual IAircraftCCallback* QueryInterface(LPCSTR pszInterface) override;

	virtual IGaugeCCallback* CreateGaugeCCallback() override;

	virtual void Update() override;

private:
	UINT32 mContainerId;

	DECLARE_PANEL_CALLBACK_REFCOUNT(RadarAircraftCallback);
};

#endif // RADAR_AIRCRAFT_CALLBACK_H
