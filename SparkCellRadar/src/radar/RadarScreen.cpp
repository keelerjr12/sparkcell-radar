#include "RadarScreen.h"
#include "Radar.h"
#include "../ui/VirtualDisplay.h"

namespace SparkCell {

	RadarScreen::RadarScreen(VirtualDisplay* vd, const Radar* const radar) : vd_(vd), radar_(radar), rng_lbl_(L"", vd) {
		Setup();
	}

	static std::wstring parse_aspect_angle_str(float aspect_angle) {
		auto suffix = 'R';
		if (aspect_angle < 0) {
			suffix = 'L';
		}

		const auto trunc_aa = static_cast<int>(std::abs(std::round(aspect_angle)) / 10);
		auto aa_str = std::to_wstring(trunc_aa);
		aa_str.push_back(suffix);

		return aa_str;
	}

	static void RenderAzimuthScale(VirtualDisplay& vd_) {
		constexpr auto NUM_TICKS_PER_SIDE = 3;
		vd_.DrawLine(0, -.72, 0, -.8172);

		for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
			vd_.DrawLine(tick * .16667, -.7554, tick * .16667, -.8172);
			vd_.DrawLine(-1 * tick * .16667, -.7554, -1 * tick * .16667, -.8172);
		}

	}

	static void RenderElevationScale(VirtualDisplay& vd_) {
		constexpr auto NUM_TICKS_PER_SIDE = 3;
		vd_.DrawLine(-.72, 0, -.8172, 0);

		for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
			vd_.DrawLine(-.7554, tick * .16667, -.8172, tick * .16667);
			vd_.DrawLine(-.7554, -1 * tick * .16667, -.8172, -1 * tick * .16667);
		}
	}

	static void RenderRangeScale(VirtualDisplay& vd_) {
		vd_.DrawLine(.932, -.5, 1, -.5);
		vd_.DrawLine(.932, 0, 1, 0);
		vd_.DrawLine(.932, .5, 1, .5);
	}

	static void RenderHorizonLine(float bank, VirtualDisplay& vd_) {
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

		vd_.DrawLine(tt[4], tt[5], tu[4], tu[5]);
		vd_.DrawLine(-1 * tt[4], -1 * tt[5], -1 * tu[4], -1 * tu[5]);

		Gdiplus::Matrix out_up(0, 0, 0, 0, -.4971, 0);
		out_up.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
		out_up.GetElements(tt);
		Gdiplus::Matrix out_bot(0, 0, 0, 0, -.4971, -.0571);
		out_bot.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
		out_bot.GetElements(tu);

		vd_.DrawLine(tt[4], tt[5], tu[4], tu[5]);
		vd_.DrawLine(tu[4] - 2 * tt[4], tu[5] - 2 * tt[5], -tt[4], -tt[5]);

	}

	static void RenderTargets(VirtualDisplay& vd, const Radar& radar) {
		const auto width = .0315;
		const auto height =.0315;

		auto targets = radar.GetRadarTargets();

		for (const auto& target : targets) {
			auto x = (target.getATA() / radar.GetAzimuth()) - (width / 2.0);
			auto y = (target.getRange() * 2 / radar.GetRange()) - 1 + (height / 2.0);
			vd.SetBrush(Gdiplus::Color::White);
			vd.DrawRect(x, y, width, height);

			// alt label per target
			const auto altitude = std::to_wstring(static_cast<int>(target.getAltitude() / 1000));
			vd.DrawString(altitude, x + width / 2.f, y - height, HJustify::CENTER, VJustify::TOP);
		}
	}

	static void RenderCursors(VirtualDisplay& vd, const Radar& radar) {
		const auto width = .0315;
		const auto height =.0315;

		auto x_T = (static_cast<float>(radar.GetCursorAzimuth()) / radar.GetAzimuth()) - (width / 2.0);
		auto y_T = (static_cast<float>(radar.GetCursorRange()) * 2 / radar.GetRange()) - 1 - (height / 2.0);
	;
		const auto width_T = .0622;
		const auto height_T = .0719;
		vd.DrawLine(x_T - width_T / 2.f, y_T - height / 2.f, x_T - width_T / 2.f, y_T + height_T / 2.f);
		vd.DrawLine(x_T + width_T / 2.f, y_T - height / 2.f, x_T + width_T / 2.f, y_T + height_T / 2.f);

	}

	void RadarScreen::Render() {
		vd_->Clear();

		for (const auto& lbl : top_lbls_) {
			lbl.Render();
		}

		for (const auto& lbl : bottom_lbls_) {
			lbl.Render();
		}
		
		// TODO: Move this to a free function so Render() can worry just about Rendering()
		const auto range = std::to_wstring(radar_->GetRange());
		rng_lbl_.SetText(range);

		auto rng_bx = rng_lbl_.BoundingBox();
		rng_bx.MoveCenter(-.92, .5);
		rng_lbl_.Move(rng_bx.X(), rng_bx.Y());

		rng_lbl_.Render();

		vd_->DrawString(L"A", -1, 0, HJustify::LEFT, VJustify::BOTTOM);
		/*az_lbl.SetText(L"A");
		auto az_lbl = az_lbl.BoundingBox();
		az_lbl.MoveCenter(-.92, .5);
		rng_lbl_.Move(rng_bx.X(), rng_bx.Y());
		rng_lbl_.Render();*/

		const auto azLbl = std::to_wstring(radar_->GetAzimuth() / 10);
		vd_->DrawString(azLbl, -1, 0, HJustify::LEFT, VJustify::TOP);

		const auto elLbl = std::to_wstring(radar_->GetBarSetting());
		vd_->DrawString(elLbl, -1, -.5, HJustify::LEFT, VJustify::BOTTOM);
		vd_->DrawString(L"B", -1, -.5, HJustify::LEFT, VJustify::TOP);

		// Render locked target params
		if (radar_->GetLockedTarget()) {
			const auto aspect = parse_aspect_angle_str(radar_->GetLockedTarget()->getAA());
			vd_->DrawString(aspect, -.8171, .897);

			const auto track = std::to_wstring(static_cast<int>(radar_->GetLockedTarget()->getHeading())); // TODO: this should actuall be rounded to nearest 10
			vd_->DrawString(track, -.6571, .897);

			const auto airspeed = std::to_wstring(static_cast<int>(radar_->GetLockedTarget()->getAirspeed())); // TODO: this should actuall be rounded to nearest 10
			vd_->DrawString(airspeed, .8171, .897, HJustify::RIGHT);
		}

		RenderAzimuthScale(*vd_);

		RenderElevationScale(*vd_);

		RenderRangeScale(*vd_);

		RenderHorizonLine(radar_->Host().bank(), *vd_);

		RenderTargets(*vd_, *radar_);

		RenderCursors(*vd_, *radar_);

	}

	void RadarScreen::Setup() {
		vd_->SetFontSize(vd_->DisplayBox().Width() * .0371);

		const auto inc = 2.f / (top_lbl_strs_.size() + 1);
		auto x = -1.f;
		for (const auto& text : top_lbl_strs_) {
		   x += inc;

		   SparkCell::Label lbl(text, vd_);

		   auto box = lbl.BoundingBox();
		   box.MoveCenter(x, 1.f);
		   box.MoveTop(1.f);
		   lbl.Move(box.X(), box.Y());

		   top_lbls_.emplace_back(std::move(lbl));
		}

		x = -1.f;
		for (const auto& text : bottom_lbl_strs_) {
		   x += inc;

		   SparkCell::Label lbl(text, vd_);

		   auto box = lbl.BoundingBox();
		   box.MoveCenter(x, 1.f);
		   box.MoveBottom(-1.f);
		   lbl.Move(box.X(), box.Y());

		   bottom_lbls_.emplace_back(std::move(lbl));
		}

		bottom_lbls_[1].SetForeground(Gdiplus::Color::Black);
		bottom_lbls_[1].SetBackground(Gdiplus::Color::White);

	}
}