#include "Rect.h"

namespace SparkCell {

	Rect::Rect(float x, float y, float width, float height) : x_(x), y_(y), width_(width), height_(height) { }

	void Rect::MoveCenter(float x, float y) {
		x_ = x - Width() / 2;
		y_ = y + Height() / 2;
	}

}