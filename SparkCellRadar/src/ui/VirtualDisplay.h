#ifndef VIRTUAL_DISPLAY_H
#define VIRTUAL_DISPLAY_H

#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#include <string>

namespace SparkCell {

	class Rect;

	enum class HJustify {
		LEFT,
		CENTER,
		RIGHT
	};

	enum class VJustify {
		TOP,
		CENTER,
		BOTTOM
	};

	class VirtualDisplay {

	public:

		VirtualDisplay(HDC hdc, int x_sz, int y_sz);

		void Clear();

		void DrawString(const std::wstring& txt, float x_pos, float y_pos, HJustify h_just = HJustify::LEFT, VJustify v_just = VJustify::TOP);

		void DrawLine(float x1, float y1, float x2, float y2);

		void DrawRect(float x, float y, float width, float height);
		void DrawRect(const SparkCell::Rect& rect);

		void SetFontSize(float fnt_sz);
		void SetBrush(const Gdiplus::Color& color);
		void SetPen(const Gdiplus::Color& color);

		Rect DisplayBox() const;
		Rect FontBoundingBox(const std::wstring& txt) const;
		

	private:

		Gdiplus::Graphics m_graphics;

		int m_x_sz;
		int m_y_sz;

		Gdiplus::PrivateFontCollection m_pvc;
		std::unique_ptr<Gdiplus::Font> m_fnt;
		std::unique_ptr<Gdiplus::Pen> m_pen;
		std::unique_ptr<Gdiplus::Brush> m_brush;
	};

}

#endif // VIRTUAL_DISPLAY_H
