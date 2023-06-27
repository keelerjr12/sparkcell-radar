#include "VirtualDisplay.h"
#include "Rect.h"

namespace SparkCell {

	VirtualDisplay::VirtualDisplay(HDC hdc, int x_sz, int y_sz) :
		m_graphics(hdc),
		m_x_sz(x_sz),
		m_y_sz(y_sz),
		m_fnt(std::make_unique<Gdiplus::Font>(L"Times New Roman", 1.0)),
		m_brush(std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color::White)),
		m_pen(std::make_unique<Gdiplus::Pen>(Gdiplus::Color::White, 1.f)) { }

	void VirtualDisplay::Clear() {
		m_graphics.Clear(Gdiplus::Color::Transparent);
	}

	void VirtualDisplay::DrawString(const std::wstring& txt, float x_pos, float y_pos, HJustify h_just, VJustify v_just) {

		const auto box = FontBoundingBox(txt);
		auto x_adj = 0.f;
		auto y_adj = 0.f;

		switch (h_just) {
		case HJustify::CENTER:
			x_adj = box.Width() / 2.f;
			break;
		case HJustify::RIGHT:
			x_adj = box.Width();
			break;
		};

		switch (v_just) {
		case VJustify::CENTER:
			y_adj = box.Height() / 2.f;
			break;
		case VJustify::BOTTOM:
			y_adj = box.Height();
			break;
		};

		auto x_px = m_x_sz * (((1 + x_pos) / 2) - x_adj);
		auto y_px = m_y_sz * (((1 - y_pos) / 2) - y_adj);

		m_graphics.DrawString(txt.c_str(), -1, m_fnt.get(), { x_px, y_px }, m_brush.get());
	}

	void VirtualDisplay::DrawLine(float x1, float y1, float x2, float y2) {
		const auto x1_px = m_x_sz * (1 + x1) / 2;
		const auto y1_px = m_y_sz * (1 - y1) / 2;
		const auto x2_px = m_x_sz * (1 + x2) / 2;
		const auto y2_px = m_y_sz * (1 - y2) / 2;

		m_graphics.DrawLine(m_pen.get(), Gdiplus::Point(x1_px, y1_px), Gdiplus::Point(x2_px, y2_px));
	}

	void VirtualDisplay::DrawRect(float x, float y, float width, float height) {
		const auto x_px = m_x_sz * (1 + x) / 2;
		const auto y_px = m_y_sz * (1 - y) / 2;
		const auto w_px = m_x_sz / 2.f * width;
		const auto h_px = m_y_sz / 2.f * height;

		m_graphics.FillRectangle(m_brush.get(), x_px, y_px, w_px, h_px);
	}

	void VirtualDisplay::DrawRect(const Rect& rect) {
		DrawRect(rect.X(), rect.Y(), rect.Width(), rect.Height());
	}

	void VirtualDisplay::SetFontSize(float fnt_sz) {
		m_pvc.AddFontFile(L"C:\\Users\\ADMIN\\Fonts\\B612\\B612-Regular.ttf");
		m_fnt = std::make_unique<Gdiplus::Font>(L"B612", fnt_sz, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel, &m_pvc);
	}

	void VirtualDisplay::SetBrush(const Gdiplus::Color& color) {
		m_brush = std::make_unique<Gdiplus::SolidBrush>(color);
	}

	Rect VirtualDisplay::DisplayBox() const {
		return Rect{ 0.f, 0.f, static_cast<float>(m_x_sz), static_cast<float>(m_y_sz)};
	}

	Rect VirtualDisplay::FontBoundingBox(const std::wstring& txt) const {
		Gdiplus::RectF box;
		m_graphics.MeasureString(txt.c_str(), txt.size(), m_fnt.get(), { 0, 0 }, &box);

		const auto bb = Rect{ 0, 0, 2 * box.Width / m_x_sz, 2 * box.Height / m_y_sz };

		return bb;
	}

}
