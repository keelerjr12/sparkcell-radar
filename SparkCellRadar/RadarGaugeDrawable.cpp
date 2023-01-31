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

	Gdiplus::FontFamily  fontFamily(L"Times New Roman");
	Gdiplus::SolidBrush  solidBrush(Gdiplus::Color(255, 255, 255, 255));

    // Render range label
	Gdiplus::Font        rngFont(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      rngLblPoint(size.x * .02, size.y * .275);
	const auto range = std::to_wstring(mRadar->GetRange());
	graphics.DrawString(range.c_str(), -1, &rngFont, rngLblPoint, &solidBrush);

    // Render azimuth label
	Gdiplus::Font        azFont(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	const auto azLbl = std::to_wstring(mRadar->GetAzimuth() / 10);

	Gdiplus::PointF      aLblPoint(size.x * .02, size.y / 2 - 14);
	graphics.DrawString(L"A", -1, &azFont, aLblPoint, &solidBrush);

	Gdiplus::PointF      azLblPoint(size.x * .02, size.y / 2);
	graphics.DrawString(azLbl.c_str(), -1, &azFont, azLblPoint, &solidBrush);

    // Render locked target params
    if (mRadar->GetLockedTarget()) {
        Gdiplus::Font        tgtFont(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

        Gdiplus::PointF      aaPoint(size.x * .034, size.y * .03);
        const auto aspect = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAA()) / 10); // TODO: this should actuall be rounded
        graphics.DrawString(aspect.c_str(), -1, &tgtFont, aaPoint, &solidBrush);

        Gdiplus::PointF      trkPoint(size.x * .204, size.y * .03);
        const auto track = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
        graphics.DrawString(track.c_str(), -1, &tgtFont, trkPoint, &solidBrush);

        Gdiplus::PointF      asPoint(size.x * (1-.25), size.y * .03);
        const auto airspeed = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
        graphics.DrawString(airspeed.c_str(), -1, &tgtFont, asPoint, &solidBrush);
    }

    // Render azimuth scale
    Gdiplus::SolidBrush  azBrush(Gdiplus::Color::White);
    Gdiplus::Pen azPen(&azBrush, 1.0F);

    auto pxPerTenDegs = size.y / (2 * mRadar->GetAzimuth() / 10);
    auto nTicks = (mRadar->GetAzimuth() / 10) / 2;

    auto startPx = size.x / 2;
    const auto bottomPx = size.y * (1 - .092);
    const auto topPx = size.y * (1 - .127);
    graphics.DrawLine(&azPen, Gdiplus::Point(startPx, bottomPx), Gdiplus::Point(startPx, topPx - 5));

    for (auto tick = 1; tick <= nTicks; ++tick) {
        graphics.DrawLine(&azPen, Gdiplus::Point(startPx + pxPerTenDegs * tick, bottomPx), Gdiplus::Point(startPx + pxPerTenDegs * tick, topPx));
        graphics.DrawLine(&azPen, Gdiplus::Point(startPx - pxPerTenDegs * tick, bottomPx), Gdiplus::Point(startPx - pxPerTenDegs * tick, topPx));
    }

    // Render elevation scale
    Gdiplus::SolidBrush  elBrush(Gdiplus::Color::White);

    Gdiplus::Pen elPen(&elBrush, 1.0F);
    pxPerTenDegs = size.y / (2 * mRadar->GetElevation() / 10);
    nTicks = (mRadar->GetElevation() / 10) / 2;

    startPx = size.y / 2;
    const auto leftPx = size.x * .092;
    const auto rightPx = size.y * .127;
    graphics.DrawLine(&elPen, Gdiplus::Point(leftPx, startPx), Gdiplus::Point(rightPx+5, startPx));

    for (auto tick = 1; tick <= nTicks; ++tick) {
        graphics.DrawLine(&elPen, Gdiplus::Point(leftPx, startPx + pxPerTenDegs * tick), Gdiplus::Point(rightPx, startPx + pxPerTenDegs * tick));
        graphics.DrawLine(&elPen, Gdiplus::Point(leftPx, startPx - pxPerTenDegs * tick), Gdiplus::Point(rightPx, startPx - pxPerTenDegs * tick));
    }

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

        // alt label per target
        const auto altitude = std::to_wstring(static_cast<int>(target.getAltitude() / 1000));
		Gdiplus::Font        altFont(&fontFamily, 10, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      altLblPt(x, y + 10);
        graphics.DrawString(altitude.c_str(), -1, &altFont, altLblPt, &solidBrush);
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
