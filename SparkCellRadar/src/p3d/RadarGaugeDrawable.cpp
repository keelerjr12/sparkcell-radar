#include "RadarGaugeDrawable.h"
#include "../radar/RadarScreen.h"
#include "../ui/VirtualDisplay.h"
#include <memory>

RadarGaugeDrawable::RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar) 
    : radar_(radar) { }

ULONG RadarGaugeDrawable::AddRef()
{
    return 0;
}

ULONG RadarGaugeDrawable::Release()
{
    return 0;
}

FLAGS32 RadarGaugeDrawable::GetFlags()
{
    return GAUGE_DRAWABLE_FLAGS::TAKES_DC;
}

void RadarGaugeDrawable::Update()
{
}

void RadarGaugeDrawable::Show(bool on)
{
}

bool RadarGaugeDrawable::Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage)
{
    if (!radar_screen_) {
		vd_ = std::make_unique<SparkCell::VirtualDisplay>(hdc, size.x, size.y);
        radar_screen_ = std::make_unique<SparkCell::RadarScreen>(vd_.get(), radar_);
    }

    radar_screen_->Render();

    return true;
}

bool RadarGaugeDrawable::SetupDraw(PIXPOINT size, HDC hdc, PIMAGE pImage)
{
    return false;
}

bool RadarGaugeDrawable::GetDraw(IGaugeCDrawableDrawParameters* pParameters)
{
    return false;
}

