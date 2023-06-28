#include "Rect.h"

namespace SparkCell {

	Rect::Rect(float x, float y, float width, float height) : x_(x), y_(y), width_(width), height_(height) { }

	float Rect::X() const {
		return x_;
	}

	float Rect::Y() const {
		return y_;
	}

	float Rect::Width() const {
		return width_;
	}

	float Rect::Height() const {
		return height_;
	}

	float Rect::CenterX() const {
		return X() + Width() / 2;
	}

	float Rect::BottomLeftX() const {
		return X();
	}

	float Rect::BottomLeftY() const {
		return Y() - Height();
	}

	float Rect::BottomRightX() const {
		return X() + Width();
	}

	float Rect::BottomRightY() const {
		return Y() - Height();
	}

	float Rect::TopLeftX() const {
		return X();
	}

	float Rect::TopLeftY() const {
		return Y();
	}

	float Rect::TopRightX() const {
		return X() + Width();
	}

	float Rect::TopRightY() const {
		return Y();
	}

	void Rect::MoveTop(float y) {
		y_ = y;
	}

	void Rect::MoveCenter(float x, float y) {
		x_ = x - Width() / 2;
		y_ = y + Height() / 2;
	}

	void Rect::MoveBottom(float y) {
		y_ = y + height_;
	}
}