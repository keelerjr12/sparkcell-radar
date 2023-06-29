#include "RadarScreen.h"
#include "Radar.h"
#include "../ui/VirtualDisplay.h"
#include "../util/Location.h"
#include "../util/Math.h"

namespace SparkCell {

	static void RenderAzimuthScale(VirtualDisplay& vd) {
		vd.SetPen(Gdiplus::Color(94, 169, 174));

		constexpr auto NUM_TICKS_PER_SIDE = 3;
		vd.DrawLine(0, -.72, 0, -.8172);

		for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
			vd.DrawLine(tick * .16667, -.7554, tick * .16667, -.8172);
			vd.DrawLine(-1 * tick * .16667, -.7554, -1 * tick * .16667, -.8172);
		}
	}

	static void RenderElevationScale(VirtualDisplay& vd) {
		vd.SetPen(Gdiplus::Color(94, 169, 174));

		constexpr auto NUM_TICKS_PER_SIDE = 3;
		vd.DrawLine(-.72, 0, -.8172, 0);

		for (auto tick = 1; tick <= NUM_TICKS_PER_SIDE; ++tick) {
			vd.DrawLine(-.7554, tick * .16667, -.8172, tick * .16667);
			vd.DrawLine(-.7554, -1 * tick * .16667, -.8172, -1 * tick * .16667);
		}
	}

	static void RenderRangeScale(VirtualDisplay& vd) {
		vd.SetPen(Gdiplus::Color::White);

		vd.DrawLine(.932, -.5, 1, -.5);
		vd.DrawLine(.932, 0, 1, 0);
		vd.DrawLine(.932, .5, 1, .5);
	}

	static void RenderHorizonLine(float bank, VirtualDisplay& vd) {
		vd.SetPen(Gdiplus::Color::White);

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
		vd.DrawLine(-1 * tt[4], -1 * tt[5], -1 * tu[4], -1 * tu[5]);

		Gdiplus::Matrix out_up(0, 0, 0, 0, -.4971, 0);
		out_up.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
		out_up.GetElements(tt);
		Gdiplus::Matrix out_bot(0, 0, 0, 0, -.4971, -.0571);
		out_bot.Multiply(&rotation, Gdiplus::MatrixOrderAppend);
		out_bot.GetElements(tu);

		vd.DrawLine(tt[4], tt[5], tu[4], tu[5]);
		vd.DrawLine(tu[4] - 2 * tt[4], tu[5] - 2 * tt[5], -tt[4], -tt[5]);

	}

	// TODO: use the cursor position to show bullseye (SPS for now)
	static void RenderBullseye(VirtualDisplay& vd, const Radar& radar, float ac_lat, float ac_lon) {
		// auto cursor = get_cursors()
		// 

		auto bng_be = CalculateBearing(33.9872875, -98.5934719, ac_lat, ac_lon);
		auto rng_be = CalculateRange(33.9872875, -98.5934719, ac_lat, ac_lon);

		Gdiplus::Matrix m;
		m.Rotate(-bng_be);
		m.Scale(1, rng_be, Gdiplus::MatrixOrderPrepend);
		m.Translate(0, 1, Gdiplus::MatrixOrderPrepend);

		Gdiplus::REAL tt[6] = {};
		m.GetElements(tt);

		const auto ac_hdg = radar.Host().heading();
		const auto crs_az = radar.GetCursorAzimuth();
		const auto crs_rng = radar.GetCursorRange(); 

		auto const crs_bng = ac_hdg + crs_az;

		Gdiplus::Matrix n;
		n.Rotate(-crs_bng);
		n.Scale(1, crs_rng, Gdiplus::MatrixOrderPrepend);
		n.Translate(0, 1, Gdiplus::MatrixOrderPrepend);

		Gdiplus::REAL tu[6] = {};
		n.GetElements(tu);

		const auto x = tt[4] + tu[4];
		const auto y = tt[5] + tu[5];
		auto rng = static_cast<int>(std::round(std::sqrt(x * x + y * y)));
		auto rng_str = std::to_wstring(rng);

		vd.DrawString(rng_str, -.75, -.75);

		const auto ang = static_cast<int>(std::round(ToDegrees(3.14159/2 - std::atan2f(y, x))+360)) % 360;
		auto ang_str = std::to_wstring(ang);

		vd.DrawString(ang_str, -1, -.75);
	}

	static std::wstring StringifyAspectAngle(float aspect_angle) {
		auto suffix = 'R';
		if (aspect_angle < 0) {
			suffix = 'L';
		}

		const auto trunc_aa = static_cast<int>(std::abs(std::round(aspect_angle)) / 10);
		auto aa_str = std::to_wstring(trunc_aa);
		aa_str.push_back(suffix);

		return aa_str;
	}

	static void RenderLockedTargetParams(VirtualDisplay& vd, const Radar& radar) {
		vd.SetPen(Gdiplus::Color::White);

		if (radar.GetLockedTarget()) {
			const auto aspect = StringifyAspectAngle(radar.GetLockedTarget()->AA());
			vd.DrawString(aspect, -.8171, .897);

			const auto track = std::to_wstring(static_cast<int>(radar.GetLockedTarget()->Heading())); // TODO: this should actuall be rounded to nearest 10
			vd.DrawString(track, -.6571, .897);

			const auto airspeed = std::to_wstring(static_cast<int>(radar.GetLockedTarget()->Airspeed())); // TODO: this should actuall be rounded to nearest 10
			vd.DrawString(airspeed, .8171, .897, HJustify::RIGHT);
		}
	}

	static void RenderTargets(VirtualDisplay& vd, const Radar& radar) {
		vd.SetPen(Gdiplus::Color::White);

		const auto width = .0315;
		const auto height =.0315;

		auto targets = radar.GetRadarTargets();

		for (const auto& target : targets) {
			auto x = target.ATA() / radar.GetAzimuth();
			auto y = (target.Range() * 2 / radar.GetRange()) - 1;

			Rect rc{ 0, 0, width, height };
			rc.MoveCenter(x, y);

			vd.SetBrush(Gdiplus::Color::White);
			vd.DrawRect(rc);

			// alt label per target
			if (radar.IsCursorNear(target)) {
				const auto altitude = std::to_wstring(static_cast<int>(target.Altitude() / 1000));

				Label alt_lbl{ altitude, &vd };
				auto alt_rc = alt_lbl.BoundingBox();
				alt_rc.MoveCenter(rc.CenterX(), rc.Y());
				alt_rc.MoveTop(rc.Y() - rc.Height());

				vd.DrawString(altitude, alt_rc.X(), alt_rc.Y());
			}
		}
	}

	static void RenderCursors(VirtualDisplay& vd, const Radar& radar) {
		vd.SetPen(Gdiplus::Color::White);

		const auto width = .066;
		const auto height =.066;

		auto x = (static_cast<float>(radar.GetCursorAzimuth()) / radar.GetAzimuth());
		auto y = (static_cast<float>(radar.GetCursorRange()) * 2 / radar.GetRange()) - 1;
		Rect rc{ 0, 0, width, height };
		rc.MoveCenter(x, y);

		vd.DrawLine(rc.BottomLeftX(), rc.BottomLeftY(), rc.TopLeftX(), rc.TopLeftY());
		vd.DrawLine(rc.BottomRightX(), rc.BottomRightY(), rc.TopRightX(), rc.TopRightY());
	}

	RadarScreen::RadarScreen(VirtualDisplay* vd, const Radar* const radar) : 
		vd_(vd), 
		radar_(radar),
		rng_lbl_(L"", vd),
		a_lbl_(L"A", vd),
		az_lbl_(L"", vd),
		el_b_lbl_(L"", vd),
		b_lbl_(L"B", vd) {

		Setup();
	}

	void RadarScreen::Render() {
		vd_->Clear();

		for (const auto& lbl : top_lbls_) {
			lbl.Render();
		}

		for (const auto& lbl : bottom_lbls_) {
			lbl.Render();
		}
		
		rng_lbl_.Render();

		a_lbl_.Render();
		az_lbl_.Render();

		el_b_lbl_.Render();
		b_lbl_.Render();

		RenderLockedTargetParams(*vd_, *radar_);

		RenderAzimuthScale(*vd_);

		RenderElevationScale(*vd_);

		RenderRangeScale(*vd_);

		RenderHorizonLine(radar_->Host().bank(), *vd_);

		RenderBullseye(*vd_, *radar_, radar_->Host().lat(), radar_->Host().lon());

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

		//range
		const auto range = std::to_wstring(radar_->GetRange());
		rng_lbl_.SetText(range);

		auto rng_bx = rng_lbl_.BoundingBox();
		rng_bx.MoveCenter(-.92, .5);
		rng_lbl_.Move(rng_bx.X(), rng_bx.Y());

		//azimuth
		auto a_bx = a_lbl_.BoundingBox();
		a_bx.MoveBottom(0);
		a_bx.MoveLeft(-1);
		a_lbl_.Move(a_bx.X(), a_bx.Y());

		const auto az = std::to_wstring(radar_->GetAzimuth() / 10);
		az_lbl_.SetText(az);

		auto az_bx = az_lbl_.BoundingBox();
		az_bx.MoveTop(0);
		az_bx.MoveLeft(-1);
		az_lbl_.Move(az_bx.X(), az_bx.Y());

		//el bars
		const auto el_bars = std::to_wstring(radar_->GetBarSetting());
		el_b_lbl_.SetText(el_bars);

		auto el_b_bx = el_b_lbl_.BoundingBox();
		el_b_bx.MoveBottom(-.5);
		el_b_bx.MoveLeft(-1);
		el_b_lbl_.Move(el_b_bx.X(), el_b_bx.Y());

		auto b_bx = b_lbl_.BoundingBox();
		b_bx.MoveTop(-.5);
		b_bx.MoveLeft(-1);
		b_lbl_.Move(b_bx.X(), b_bx.Y());
	}
}