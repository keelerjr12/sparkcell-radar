#ifndef RADAR_GAUGE_DRAWABLE_H
#define RADAR_GAUGE_DRAWABLE_H

#include <gauges.h>
#include <gdiplus.h>
#include "Label.h"
#include <memory>
#include <string>
#include <vector>
#include "VirtualDisplay.h"

namespace SparkCell {
	class Radar;
}

enum RadarGaugeDrawableId {
	RADAR
};

class RadarGaugeDrawable : public IGaugeCDrawable {
public:
	RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar);

	// Inherited via IGaugeCDrawable
	virtual ULONG AddRef() override;
	virtual ULONG Release() override;
	virtual FLAGS32 GetFlags() override;
	virtual void Update() override;
	virtual void Show(bool on) override;
	virtual bool Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool GetDraw(IGaugeCDrawableDrawParameters* pParameters) override;

private:
	const SparkCell::Radar* const mRadar;
	std::unique_ptr<SparkCell::VirtualDisplay> vd;

	std::vector<std::wstring> top_lbl_strs_ { L"CRM", L"RWS", L"NORM", L"OVRD", L"CNTL" };
	std::vector<std::wstring> bottom_lbl_strs_ { L"SWAP", L"FCR", L"HSD", L"WPN", L"DCLT" };

	std::vector<SparkCell::Label> top_lbls_;
	std::vector<SparkCell::Label> bottom_lbls_;
};

#endif // RADAR_GAUGE_DRAWABLE_H
