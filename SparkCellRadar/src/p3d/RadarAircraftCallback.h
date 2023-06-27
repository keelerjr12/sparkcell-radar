#ifndef RADAR_AIRCRAFT_CALLBACK_H
#define RADAR_AIRCRAFT_CALLBACK_H

#include "../radar/Aircraft.h"
#include <gauges.h>

class RadarAircraftCallback : public IAircraftCCallback {
public:
	RadarAircraftCallback(UINT32 containerId);

	virtual IAircraftCCallback* QueryInterface(LPCSTR pszInterface) override;

	virtual IGaugeCCallback* CreateGaugeCCallback() override;

	virtual void Update() override;

private:
	UINT32 mContainerId;

	SparkCell::Aircraft user_aircraft;

	DECLARE_PANEL_CALLBACK_REFCOUNT(RadarAircraftCallback);
};

#endif // RADAR_AIRCRAFT_CALLBACK_H
