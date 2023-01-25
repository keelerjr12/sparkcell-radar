#ifndef RADAR_GAUGE_CALLBACK_H
#define RADAR_GAUGE_CALLBACK_H

#include <gauges.h>

class RadarGaugeCallback : public IGaugeCCallback {
public:
	RadarGaugeCallback(UINT32 containerId) : m_RefCount(1), mContainerId(containerId) { }

	virtual IGaugeCCallback* QueryInterface(LPCSTR pszInterface) override;
	virtual void Update() override;
	virtual bool GetPropertyValue(SINT32 id, FLOAT64* pValue) override;
	virtual bool GetPropertyValue(SINT32 id, LPCSTR* pszValue) override;
	virtual bool GetPropertyValue(SINT32 id, LPCWSTR* pszValue) override;
	virtual bool SetPropertyValue(SINT32 id, FLOAT64 value) override;
	virtual bool SetPropertyValue(SINT32 id, LPCSTR szValue) override;
	virtual bool SetPropertyValue(SINT32 id, LPCWSTR szValue) override;
	virtual IGaugeCDrawable* CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters) override;

private:
	UINT32 mContainerId;

	DECLARE_PANEL_CALLBACK_REFCOUNT(RadarGaugeCallback);
};

#endif // RADAR_GAUGE_CALLBACK_H 
