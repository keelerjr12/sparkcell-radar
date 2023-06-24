#ifndef RECT_H
#define RECT_H

namespace SparkCell {

	class Rect {
	public:
		Rect(float x, float y, float width, float height);

		float X() const { return x_; }
		float Y() const { return y_; }
		float Width() const { return width_; }
		float Height() const { return height_; }

		void MoveCenter(float x, float y);

	private:
		float x_;
		float y_;
		float width_;
		float height_;
	};

}

#endif // RECT_H
