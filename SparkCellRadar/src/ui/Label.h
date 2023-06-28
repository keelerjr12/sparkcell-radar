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
		Label(const std::wstring& text = L"", VirtualDisplay* vd = nullptr);

		void Move(float x, float y);
		void SetHAlign(HJustify h_align);
		void SetVAlign(VJustify v_align);

		Gdiplus::Color Background() const;
		void SetBackground(const Gdiplus::Color& color);
		void SetForeground(const Gdiplus::Color& color); 

		void SetText(const std::wstring& text);

		Rect BoundingBox() const;

		void Render() const;

	private:

		std::wstring text_;

		float x_ = {0.f};
		float y_ = {0.f};

		HJustify h_align_ = { HJustify::LEFT };
		VJustify v_align_ = { VJustify::CENTER };

		Gdiplus::Color bkgd_;
		Gdiplus::Color fgd_;

		VirtualDisplay* vd_;
	};

}

#endif