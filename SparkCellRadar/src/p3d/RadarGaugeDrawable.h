#ifndef RADAR_GAUGE_DRAWABLE_H
#define RADAR_GAUGE_DRAWABLE_H

#include <gauges.h>
#include <memory>

namespace SparkCell {
	class Radar;
	class RadarScreen;
	class VirtualDisplay;
}

enum RadarGaugeDrawableId {
	RADAR
};

class RadarGaugeDrawable : public IGaugeCDrawable {
public:
	RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar);

	virtual ULONG AddRef() override;
	virtual ULONG Release() override;
	virtual FLAGS32 GetFlags() override;
	virtual void Update() override;
	virtual void Show(bool on) override;
	virtual bool Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage) override;
	virtual bool GetDraw(IGaugeCDrawableDrawParameters* pParameters) override;

private:

	std::unique_ptr<SparkCell::VirtualDisplay> vd_;
	std::unique_ptr<SparkCell::RadarScreen> radar_screen_;
	const SparkCell::Radar* const radar_;

};

#endif // RADAR_GAUGE_DRAWABLE_H
