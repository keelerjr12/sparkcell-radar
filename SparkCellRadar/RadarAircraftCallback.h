#ifndef RADAR_AIRCRAFT_CALLBACK_H
#define RADAR_AIRCRAFT_CALLBACK_H

#include <gauges.h>

class RadarAircraftCallback : public IAircraftCCallback {
public:

	// Inherited via IAircraftCCallback
	virtual ULONG AddRef() override;
	virtual ULONG Release() override;
	virtual IAircraftCCallback* QueryInterface(LPCSTR pszInterface) override;
	virtual IGaugeCCallback* CreateGaugeCCallback() override;
	virtual void Update() override;
};

#endif // RADAR_AIRCRAFT_CALLBACK_H
