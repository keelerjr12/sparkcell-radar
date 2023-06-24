#include "Label.h"
#include "Rect.h"
#include "VirtualDisplay.h"

namespace SparkCell {
	Label::Label(const std::wstring& text, VirtualDisplay& vd) 
		: text_(text),
		  vd_(&vd),
		  bkgd_(std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color::Transparent)) { }

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
		bkgd_ = std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color::White);
	}

	Rect Label::BoundingBox() const {
		return vd_->FontBoundingBox(text_);
	}

	// TODO: remove VD
	void Label::Render(VirtualDisplay& vd) const {
		auto box = vd_->FontBoundingBox(text_);

		vd.DrawRect(*bkgd_.get(), x_, y_, box.Width(), box.Height());
		vd.DrawString(text_.c_str(), x_, y_);
	}

}