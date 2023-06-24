#include "Label.h"
#include "Rect.h"
#include "VirtualDisplay.h"

namespace SparkCell {
	Label::Label(const std::wstring& text) {
		text_ = text;
	}

	void Label::Move(float x, float y) {
		x_ = x;
		y_ = y;
	}

	void Label::SetHAlign(HJustify h_align) {
		h_align_ = h_align;
	}

	void Label::SetVAlign(VJustify v_align) {
		v_align_ = v_align;
	}

	const Gdiplus::Brush& Label::Background() const {
		return *bkgd_;
	}

	void Label::SetBackground() {
		//Gdiplus::SolidBrush (Gdiplus::Color(255, 255, 255, 255));
		bkgd_ = std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color::White);
	}

	void Label::Render(VirtualDisplay& vd) const {
		auto box = vd.FontBoundingBox(text_);

		box.MoveCenter(x_, y_);

		vd.DrawRect(box);
		vd.DrawString(text_.c_str(), x_, y_, h_align_, v_align_);
	}

}