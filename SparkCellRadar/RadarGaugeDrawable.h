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

	std::vector<SparkCell::Label> top_lbls_;/* = {
		std::move(SparkCell::Label{L"CRM"}) };/*,
		std::move(SparkCell::Label{L"RWS"}), 
		std::move(SparkCell::Label{L"NORM"}),
		std::move(SparkCell::Label{L"OVRD"}),
		std::move(SparkCell::Label{L"CNTL"})
	};*/

	std::vector<SparkCell::Label> bottom_lbls_; /* = {
		std::move(SparkCell::Label {L"SWAP"}), 
		std::move(SparkCell::Label{L"FCR"}), 
		std::move(SparkCell::Label{L"HSD"}),
		std::move(SparkCell::Label{L"WPN"}),
		std::move(SparkCell::Label{L"DCLT"})
	};*/
};

#endif // RADAR_GAUGE_DRAWABLE_H
