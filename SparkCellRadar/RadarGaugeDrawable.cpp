#include "RadarGaugeDrawable.h"
#include "Radar.h"
#include <cmath>
#include <fstream>
#include <gdiplus.h>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>


RadarGaugeDrawable::RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar) : mRadar(radar), vd(std::unique_ptr<VirtualDisplay>(nullptr)) {
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
    if (!vd) {
        vd = std::make_unique<VirtualDisplay>(hdc, size.x, size.y);
    }

    vd->Clear();

    vd->SetFontSize(size.x * .0371);
    vd->DrawString(L"CRM", .16667, 0, HJustify::CENTER);
    vd->DrawString(L"RWS", .33333, 0, HJustify::CENTER);
    vd->DrawString(L"NORM", .5, 0, HJustify::CENTER);
    vd->DrawString(L"OVRD", .66667, 0, HJustify::CENTER);
    vd->DrawString(L"CNTL", .83333, 0, HJustify::CENTER);

    vd->DrawString(L"SWAP", .16667, 1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"FCR", .33333, 1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"HSD", .5, 1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"WPN", .66667, 1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"DCLT", .83333, 1, HJustify::CENTER, VJustify::BOTTOM);
    

	const auto range = std::to_wstring(mRadar->GetRange());
    vd->DrawString(range, .02, .275);

	//Gdiplus::PointF      aLblPoint(size.x * .02, size.y / 2 - font_sz);
	//graphics.DrawString(L"A", -1, &azFont, aLblPoint, &solidBrush);
    //vd->DrawString(graphics, L"A", .02, .275);

	const auto azLbl = std::to_wstring(mRadar->GetAzimuth() / 10);
    vd->DrawString(azLbl, .02, .5);

    // Render locked target params
    if (mRadar->GetLockedTarget()) {
        const auto aspect = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAA()) / 10); // TODO: this should actuall be rounded
        vd->DrawString(aspect, .094, .0514);

        const auto track = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(track, .171, .0514);

        const auto airspeed = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(airspeed, .643, .0514);
    }

    // Render azimuth scale
    auto nTicks = (mRadar->GetAzimuth() / 10) / 2;
    vd->DrawLine(.5, .86, .5, .9086);

    for (auto tick = 1; tick <= nTicks; ++tick) {
        vd->DrawLine(.5 + tick * .083333 , .8772, .5 + tick * .083333, .9086);
        vd->DrawLine(.5 - tick * .083333 , .8772, .5 - tick * .083333, .9086);
    }

    // Render elevation scale
    const auto pxPerTenDegs = size.y / (2 * mRadar->GetElevation() / 10);
    nTicks = (mRadar->GetElevation() / 10) / 2;
    vd->DrawLine(.0914, .5, .14, .5);

    for (auto tick = 1; tick <= nTicks; ++tick) {
        vd->DrawLine(.0914, .5 + tick * .083333 , .1228, .5 + tick * .083333);
        vd->DrawLine(.0914, .5 - tick * .083333 , .1228, .5 - tick * .083333);
    }

    // Render horizon line
    const auto bank_angle = 45 * std::numbers::pi / 180;
    const auto max_wg_len = .25;
    const auto init_wg_len = .04;
    const auto x1_offset = max_wg_len * std::cosf(bank_angle);
    const auto y1_offset = max_wg_len * std::sinf(bank_angle);
    const auto x2_offset = init_wg_len * std::cosf(bank_angle);
    const auto y2_offset = init_wg_len * std::sinf(bank_angle);

    const auto x1 = .5 - x1_offset;
    const auto y1 = .5 - y1_offset;
    const auto x2 = .5 - x2_offset;
    const auto y2 = .5 - y2_offset;
    vd->DrawLine(x1, y1, x2, y2);

    // Render targets
    const auto width = .0315;
    const auto height =.0315;

    auto targets = mRadar->GetRadarTargets();

    for (const auto& target : targets) {
        auto x = (target.getATA() / (2 * mRadar->GetAzimuth())) + .5 - (width / 2.0);
        auto y = 1 - (target.getRange() / mRadar->GetRange()) - (height / 2.0);
        vd->DrawRect(x, y, width, height);

        // alt label per target
        const auto altitude = std::to_wstring(static_cast<int>(target.getAltitude() / 1000));
        vd->DrawString(altitude, x + width / 2, y + .0486, HJustify::CENTER);
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
