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
    vd->DrawString(range, -.96, .45, HJustify::CENTER, VJustify::CENTER);

	//Gdiplus::PointF      aLblPoint(size.x * .02, size.y / 2 - font_sz);
	//graphics.DrawString(L"A", -1, &azFont, aLblPoint, &solidBrush);
    //vd->DrawString(graphics, L"A", .02, .275);

	const auto azLbl = std::to_wstring(mRadar->GetAzimuth() / 10);
    vd->DrawString(azLbl, -.96, 0, HJustify::CENTER, VJustify::CENTER);

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
        vd->DrawLine(.0914, .5 + tick * .083333 , .1228, .5 + tick * .083333);
        vd->DrawLine(.0914, .5 - tick * .083333 , .1228, .5 - tick * .083333);
    }

    // Render range scale
    vd->DrawLine(.966, .75, 1, .75);
    vd->DrawLine(.966, .5, 1, .5);
    vd->DrawLine(.966, .25, 1, .25);


    // Render horizon line

    Gdiplus::Matrix rot_mat(.04, .25, 0, 0, 0, 0);
    rot_mat.RotateAt(-1*mRadar->Host().bank(), { 0, 0 });
    Gdiplus::REAL rot_mat_els[6] = {};
    rot_mat.GetElements(rot_mat_els);

    Gdiplus::Matrix outer_rot_mat(.25, .25, 0, -.03, 0, 0);
    outer_rot_mat.RotateAt(-1*mRadar->Host().bank(), { 0, 0 });
    Gdiplus::REAL outer_rot_mat_els[6] = {};
    outer_rot_mat.GetElements(outer_rot_mat_els);


    vd->DrawLine(.5 - rot_mat_els[0], .5 - rot_mat_els[2], .5 - rot_mat_els[1], .5 - rot_mat_els[3]);
   // vd->DrawLine(.5 + rot_mat_els[0], .5 - rot_mat_els[2], .5 + rot_mat_els[1], .5 - rot_mat_els[3]);

    vd->DrawLine(.5 - outer_rot_mat_els[0], .5 - outer_rot_mat_els[2], .5 - outer_rot_mat_els[1], .5 - outer_rot_mat_els[3]);
    //vd->DrawLine(.5 + x1_offset, .5 + y1_offset, .5 + x2_offset, .5 + y2_offset);

    Gdiplus::REAL tt[6] = {};
    Gdiplus::Matrix a(0, 0, 0, 0, .25, .25);

    Gdiplus::Matrix translation(1, 0, 0, 1, -.5, -.5);
    a.Multiply(&translation, Gdiplus::MatrixOrderAppend);
    a.GetElements(tt);

    Gdiplus::Matrix rotation;
    rotation.Rotate(45, Gdiplus::MatrixOrderAppend);

    a.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    a.GetElements(tt);

    Gdiplus::Matrix translation_inv(1, 0, 0, 1, .5, .5);
    a.Multiply(&translation_inv, Gdiplus::MatrixOrderAppend);

    a.GetElements(tt);

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
