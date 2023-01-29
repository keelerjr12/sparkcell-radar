#include "RadarGaugeDrawable.h"
#include "Radar.h"
#include <gdiplus.h>
#include <iostream>
#include <string>

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

    // Render aspect
	Gdiplus::FontFamily  fontFamily(L"Times New Roman");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));

	Gdiplus::PointF      aaPoint(30.0f, 8.0f);
	const auto aspect = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAA()) / 10); // TODO: this should actuall be rounded
	graphics.DrawString(aspect.c_str(), -1, &font, aaPoint, &solidBrush);

	Gdiplus::PointF      trkPoint(60.0f, 8.0f);
	const auto track = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
	graphics.DrawString(track.c_str(), -1, &font, trkPoint, &solidBrush);

	Gdiplus::PointF      asPoint(120.0f, 8.0f);
	const auto airspeed = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
	graphics.DrawString(airspeed.c_str(), -1, &font, asPoint, &solidBrush);

    // Render targets
    auto width = 10.0;
    auto height = 10.0;
    Gdiplus::SolidBrush brush(Gdiplus::Color::White);

    auto pixelsPerAzDeg = size.x / (2 * mRadar->GetAzimuth());
    auto pixelsPerRngMile = size.y / mRadar->GetRange();

    auto targets = mRadar->GetRadarTargets();

    for (const auto& target : targets) {
        Gdiplus::REAL x = ((size.x - width) / 2.0) + (target.getATA() * pixelsPerAzDeg);
        Gdiplus::REAL y = ((mRadar->GetRange() - target.getRange()) * pixelsPerRngMile) - (height / 2.0);
		graphics.FillRectangle(&brush, x, y, width, height);

        std::cout << target.getBearing() << ", " << target.getATA() << std::endl;
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
