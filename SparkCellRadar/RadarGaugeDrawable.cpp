#include "RadarGaugeDrawable.h"
#include "Radar.h"
#include <gdiplus.h>
#include <iostream>

RadarGaugeDrawable::RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar) : mRadar(radar) {
}

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
    std::cout << "UPDATE\n";
}

void RadarGaugeDrawable::Show(bool on)
{
}

bool RadarGaugeDrawable::Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.Clear(Gdiplus::Color::Transparent);

    Gdiplus::SolidBrush brush(Gdiplus::Color::White);
    auto width = 20.0;
    auto height = 20.0;

    auto pixelsPerAzDeg = size.x / (2 * mRadar->GetAzimuth());
    auto pixelsPerRngMile = size.y / mRadar->GetRange();

    auto hits = mRadar->GetRadarHits();

    for (const auto& hit : hits) {
        Gdiplus::REAL x = ((size.x - width) / 2.0) + (hit.bearing * pixelsPerAzDeg);
        Gdiplus::REAL y = ((mRadar->GetRange() - hit.range) * pixelsPerRngMile) - (height / 2.0);
		graphics.FillRectangle(&brush, x, y, width, height);
    }

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
