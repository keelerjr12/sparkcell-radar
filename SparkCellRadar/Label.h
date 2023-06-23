#ifndef LABEL_H
#define LABEL_H

#include <memory>
#include <string>
#include "VirtualDisplay.h"

namespace SparkCell {

	class Label {
	public:
		Label(const std::wstring& text);

		void Move(float x, float y);
		void SetHAlign(HJustify h_align);
		void SetVAlign(VJustify v_align);

		//const Gdiplus::Brush& Background() const;
		void SetBackground();

		void Render(VirtualDisplay& vd) const;

	private:

		std::wstring text_;

		float x_ = {0.f};
		float y_ = {0.f};

		HJustify h_align_ = { HJustify::LEFT };
		VJustify v_align_ = { VJustify::CENTER };

		std::unique_ptr<int> bkgd_;// = std::make_unique<int>(1);
	};

}

#endif