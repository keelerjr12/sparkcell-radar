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
    vd->DrawString(L"CRM", -.66667, 1, HJustify::CENTER);
    vd->DrawString(L"RWS", -.33333, 1, HJustify::CENTER);
    vd->DrawString(L"NORM", 0, 1, HJustify::CENTER);
    vd->DrawString(L"OVRD", .33333, 1, HJustify::CENTER);
    vd->DrawString(L"CNTL", .66667, 1, HJustify::CENTER);

    vd->DrawString(L"SWAP", -.66667,  -1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"FCR", -.33333,  -1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"HSD",  0, -1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"WPN", .33333, -1, HJustify::CENTER, VJustify::BOTTOM);
    vd->DrawString(L"DCLT", .66667, -1, HJustify::CENTER, VJustify::BOTTOM);
    

	const auto range = std::to_wstring(mRadar->GetRange());
    vd->DrawString(range, -.92, .5, HJustify::CENTER, VJustify::TOP);

    vd->DrawString(L"A", -1, 0, HJustify::LEFT, VJustify::BOTTOM);
	const auto azLbl = std::to_wstring(mRadar->GetAzimuth() / 10);
    vd->DrawString(azLbl, -1, 0, HJustify::LEFT, VJustify::TOP);

	const auto elLbl = std::to_wstring(mRadar->GetElevation() / 10);
    vd->DrawString(elLbl, -1, -.5, HJustify::LEFT, VJustify::BOTTOM);
    vd->DrawString(L"B", -1, -.5, HJustify::LEFT, VJustify::TOP);

    // Render locked target params
    if (mRadar->GetLockedTarget()) {
        const auto aspect = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAA()) / 10); // TODO: this should actuall be rounded
        vd->DrawString(aspect, -.8171, .897);

        const auto track = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(track, -.6571, .897);

        const auto airspeed = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(airspeed, .8171, .897, HJustify::RIGHT);
    }

    // Render azimuth scale
    auto nTicks = (mRadar->GetAzimuth() / 10) / 2;
    vd->DrawLine(0, -.72, 0, -.8172);

    for (auto tick = 1; tick <= nTicks; ++tick) {
        vd->DrawLine(tick * .16667 , -.7554, tick * .16667, -.8172);
        vd->DrawLine(-1 * tick * .16667 , -.7554, -1 * tick * .16667, -.8172);
    }

    // Render elevation scale
    const auto pxPerTenDegs = size.y / (2 * mRadar->GetElevation() / 10);
    nTicks = (mRadar->GetElevation() / 10) / 2;
    vd->DrawLine(-.72, 0, -.8172, 0);

    for (auto tick = 1; tick <= nTicks; ++tick) {
        vd->DrawLine(-.7554, tick * .16667 , -.8172, tick * .16667);
        vd->DrawLine(-.7554, -1 * tick * .16667 , -.8172, -1 * tick * .16667);
    }

    // Render range scale
    vd->DrawLine(.932, -.5, 1, -.5);
    vd->DrawLine(.932, 0, 1, 0);
    vd->DrawLine(.932, .5, 1, .5);


    // Render horizon line
    Gdiplus::Matrix rotation;
    rotation.Rotate(-1*mRadar->Host().bank(), Gdiplus::MatrixOrderAppend);

    Gdiplus::Matrix a(0, 0, 0, 0, -.0857, 0);
    a.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    Gdiplus::REAL tt[6] = {};
    a.GetElements(tt);
    
    Gdiplus::Matrix b(0, 0, 0, 0, -.4971, 0);
    b.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    Gdiplus::REAL tu[6] = {};
    b.GetElements(tu);

    vd->DrawLine(tt[4], tt[5], tu[4], tu[5]);
    vd->DrawLine(-1*tt[4], -1*tt[5], -1*tu[4], -1*tu[5]);

    Gdiplus::Matrix out_up(0, 0, 0, 0, -.4971, 0);
    out_up.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    out_up.GetElements(tt);
    Gdiplus::Matrix out_bot(0, 0, 0, 0, -.4971, -.0571);
    out_bot.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    out_bot.GetElements(tu);

    vd->DrawLine(tt[4], tt[5], tu[4], tu[5]);
    vd->DrawLine(tu[4] - 2 * tt[4], tu[5] - 2 * tt[5], -tt[4], -tt[5]);

    // Render targets
    const auto width = .0315;
    const auto height =.0315;

    auto targets = mRadar->GetRadarTargets();

    for (const auto& target : targets) {
        auto x = (target.getATA() / mRadar->GetAzimuth()) - (width / 2.0);
        auto y = (target.getRange() * 2 / mRadar->GetRange()) - 1 + (height / 2.0);
        vd->DrawRect(x, y, width, height);

        // alt label per target
        const auto altitude = std::to_wstring(static_cast<int>(target.getAltitude() / 1000));
		vd->DrawString(altitude, x + width / 2.f, y - height, HJustify::CENTER, VJustify::TOP);
	}

    // Render Cursors
    auto x_T = (static_cast<float>(mRadar->GetCursorAzimuth()) / mRadar->GetAzimuth()) - (width / 2.0);
    auto y_T = 0;
    const auto width_T = .03;
    const auto height_T = .03;
    vd->DrawLine(x_T - width_T / 2.f, y_T - height / 2.f, x_T - width_T / 2.f, y_T + height_T / 2.f);
    vd->DrawLine(x_T + width_T / 2.f, y_T - height / 2.f, x_T + width_T / 2.f, y_T + height_T / 2.f);


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
