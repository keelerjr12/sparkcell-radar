#include "Label.h"
#include "Rect.h"
#include "VirtualDisplay.h"

namespace SparkCell {
	Label::Label(const std::wstring& text, VirtualDisplay* vd) 
		: text_(text),
		  vd_(vd),
		  bkgd_(Gdiplus::Color::Transparent),
		  fgd_(Gdiplus::Color::White) { }

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

	const Gdiplus::Color& Label::Background() const {
		return bkgd_;
	}

	void Label::SetBackground(const Gdiplus::Color& color) {
		bkgd_ = color;
	}

	void Label::SetForeground(const Gdiplus::Color& color) {
		fgd_ = color;
	}
		
	void Label::SetText(const std::wstring& text) {
		text_ = text;
	}

	Rect Label::BoundingBox() const {
		return vd_->FontBoundingBox(text_);
	}

	void Label::SetVD(VirtualDisplay* vd) {
		vd_ = vd;
	}

	// TODO: remove VD
	void Label::Render() const {
		auto box = vd_->FontBoundingBox(text_);

		vd_->SetBrush(bkgd_);
		vd_->DrawRect(x_, y_, box.Width(), box.Height());
		
		vd_->SetBrush(fgd_);
		vd_->DrawString(text_.c_str(), x_, y_);
	}

}