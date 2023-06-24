#ifndef LABEL_H
#define LABEL_H

#include "Rect.h"
#include "VirtualDisplay.h"
#include <memory>
#include <string>

namespace SparkCell {
	class VirtualDisplay;

	class Label {
	public:
		Label(const std::wstring& text, VirtualDisplay& vd);

		void Move(float x, float y);
		void SetHAlign(HJustify h_align);
		void SetVAlign(VJustify v_align);

		const Gdiplus::Brush& Background() const;
		void SetBackground();

		Rect BoundingBox() const;

		void Render(VirtualDisplay& vd) const;

	private:

		std::wstring text_;

		float x_ = {0.f};
		float y_ = {0.f};

		HJustify h_align_ = { HJustify::LEFT };
		VJustify v_align_ = { VJustify::CENTER };

		std::unique_ptr<Gdiplus::Brush> bkgd_;
		VirtualDisplay* vd_;
	};

}

#endif