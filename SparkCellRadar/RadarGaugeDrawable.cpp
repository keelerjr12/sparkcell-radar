#include "RadarGaugeDrawable.h"
#include "Radar.h"
#include <cmath>
#include <fstream>
#include <gdiplus.h>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>

using namespace SparkCell;


RadarGaugeDrawable::RadarGaugeDrawable(const IGaugeCDrawableCreateParameters* pParams, const SparkCell::Radar* const radar) 
    : mRadar(radar), 
      vd(std::unique_ptr<VirtualDisplay>(nullptr)) { }

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

std::wstring parse_aspect_angle_str(float aspect_angle) {
    auto suffix = 'R';
    if (aspect_angle < 0) {
        suffix = 'L';
    }

    const auto trunc_aa = static_cast<int>(std::abs(std::round(aspect_angle)) / 10);
    auto aa_str = std::to_wstring(trunc_aa);
    aa_str.push_back(suffix);

    return aa_str;
}

static void RenderAzimuthScale(VirtualDisplay& vd) {
    constexpr auto NUM_TICKS_PER_SIDE = 3;
    vd.DrawLine(0, -.72, 0, -.8172);

    for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
        vd.DrawLine(tick * .16667 , -.7554, tick * .16667, -.8172);
        vd.DrawLine(-1 * tick * .16667 , -.7554, -1 * tick * .16667, -.8172);
    }

}

static void RenderElevationScale(VirtualDisplay& vd) {
    constexpr auto NUM_TICKS_PER_SIDE = 3;
    vd.DrawLine(-.72, 0, -.8172, 0);

    for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
        vd.DrawLine(-.7554, tick * .16667 , -.8172, tick * .16667);
        vd.DrawLine(-.7554, -1 * tick * .16667 , -.8172, -1 * tick * .16667);
    }
}

static void RenderRangeScale(VirtualDisplay& vd) {
    vd.DrawLine(.932, -.5, 1, -.5);
    vd.DrawLine(.932, 0, 1, 0);
    vd.DrawLine(.932, .5, 1, .5);
}

static void RenderHorizonLine(float bank, VirtualDisplay& vd) {
    const auto inv_bank = -bank;

    Gdiplus::Matrix rotation;
    rotation.Rotate(inv_bank, Gdiplus::MatrixOrderAppend);

    Gdiplus::Matrix a(0, 0, 0, 0, -.0857, 0);
    a.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    Gdiplus::REAL tt[6] = {};
    a.GetElements(tt);
    
    Gdiplus::Matrix b(0, 0, 0, 0, -.4971, 0);
    b.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    Gdiplus::REAL tu[6] = {};
    b.GetElements(tu);

    vd.DrawLine(tt[4], tt[5], tu[4], tu[5]);
    vd.DrawLine(-1*tt[4], -1*tt[5], -1*tu[4], -1*tu[5]);

    Gdiplus::Matrix out_up(0, 0, 0, 0, -.4971, 0);
    out_up.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    out_up.GetElements(tt);
    Gdiplus::Matrix out_bot(0, 0, 0, 0, -.4971, -.0571);
    out_bot.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
    out_bot.GetElements(tu);

    vd.DrawLine(tt[4], tt[5], tu[4], tu[5]);
    vd.DrawLine(tu[4] - 2 * tt[4], tu[5] - 2 * tt[5], -tt[4], -tt[5]);

}

bool RadarGaugeDrawable::Draw(IGaugeCDrawableDrawParameters* pParameters, PIXPOINT size, HDC hdc, PIMAGE pImage)
{
    Setup(size, hdc);

    vd->Clear();

    for (const auto& lbl : top_lbls_) {
        lbl.Render();
    }

    for (const auto& lbl : bottom_lbls_) {
        lbl.Render();
    }
    
	const auto range = std::to_wstring(mRadar->GetRange());
    rng_lbl.SetText(range);

    auto rng_bx = rng_lbl.BoundingBox();
    rng_bx.MoveCenter(-.92, .5);
    rng_lbl.Move(rng_bx.X(), rng_bx.Y());

    rng_lbl.Render();

    vd->DrawString(L"A", -1, 0, HJustify::LEFT, VJustify::BOTTOM);
    /*az_lbl.SetText(L"A");
    auto az_lbl = az_lbl.BoundingBox();
    az_lbl.MoveCenter(-.92, .5);
    rng_lbl.Move(rng_bx.X(), rng_bx.Y());
    rng_lbl.Render();*/

	const auto azLbl = std::to_wstring(mRadar->GetAzimuth() / 10);
    vd->DrawString(azLbl, -1, 0, HJustify::LEFT, VJustify::TOP);

	const auto elLbl = std::to_wstring(mRadar->GetElevation() / 10);
    vd->DrawString(elLbl, -1, -.5, HJustify::LEFT, VJustify::BOTTOM);
    vd->DrawString(L"B", -1, -.5, HJustify::LEFT, VJustify::TOP);

    // Render locked target params
    if (mRadar->GetLockedTarget()) {
        const auto aspect = parse_aspect_angle_str(mRadar->GetLockedTarget()->getAA());
        vd->DrawString(aspect, -.8171, .897);

        const auto track = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(track, -.6571, .897);

        const auto airspeed = std::to_wstring(static_cast<int>(mRadar->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
        vd->DrawString(airspeed, .8171, .897, HJustify::RIGHT);
    }

    RenderAzimuthScale(*vd.get());

    RenderElevationScale(*vd.get());

    RenderRangeScale(*vd.get());

    RenderHorizonLine(mRadar->Host().bank(), *vd.get());

    // Render targets
    const auto width = .0315;
    const auto height =.0315;

    auto targets = mRadar->GetRadarTargets();

    for (const auto& target : targets) {
        auto x = (target.getATA() / mRadar->GetAzimuth()) - (width / 2.0);
        auto y = (target.getRange() * 2 / mRadar->GetRange()) - 1 + (height / 2.0);
        vd->SetBrush(Gdiplus::Color::White);
        vd->DrawRect(x, y, width, height);

        // alt label per target
        const auto altitude = std::to_wstring(static_cast<int>(target.getAltitude() / 1000));
		vd->DrawString(altitude, x + width / 2.f, y - height, HJustify::CENTER, VJustify::TOP);
	}

    // Render Cursors
    auto x_T = (static_cast<float>(mRadar->GetCursorAzimuth()) / mRadar->GetAzimuth()) - (width / 2.0);
    auto y_T = (static_cast<float>(mRadar->GetCursorRange()) * 2 / mRadar->GetRange()) - 1 - (height / 2.0);
;
    const auto width_T = .0622;
    const auto height_T = .0719;
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

void RadarGaugeDrawable::Setup(PIXPOINT size, HDC hdc) {
    if (vd) {
        return;
    }

    vd = std::make_unique<VirtualDisplay>(hdc, size.x, size.y);
    rng_lbl.SetVD(vd.get());

    vd->SetFontSize(vd->DisplayBox().Width() * .0371);

    const auto inc = 2.f / (top_lbl_strs_.size() + 1);
    auto x = -1.f;
    for (const auto& text : top_lbl_strs_) {
       x += inc;

       SparkCell::Label lbl(text, vd.get());

       auto box = lbl.BoundingBox();
       box.MoveCenter(x, 1.f);
       box.MoveTop(1.f);
       lbl.Move(box.X(), box.Y());

       top_lbls_.emplace_back(std::move(lbl));
    }

    x = -1.f;
    for (const auto& text : bottom_lbl_strs_) {
       x += inc;

       SparkCell::Label lbl(text, vd.get());

       auto box = lbl.BoundingBox();
       box.MoveCenter(x, 1.f);
       box.MoveBottom(-1.f);
       lbl.Move(box.X(), box.Y());

       bottom_lbls_.emplace_back(std::move(lbl));
    }

    bottom_lbls_[1].SetForeground(Gdiplus::Color::Black);
    bottom_lbls_[1].SetBackground(Gdiplus::Color::White);

}
