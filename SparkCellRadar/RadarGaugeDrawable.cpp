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

    Gdiplus::Matrix t_rot_mat{ 1, 0, 0, 1, 0, 0};
    t_rot_mat.RotateAt(90, { .5, .5 });
    Gdiplus::REAL t_rot_mat_els[6] = {};
    t_rot_mat.GetElements(t_rot_mat_els);


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
