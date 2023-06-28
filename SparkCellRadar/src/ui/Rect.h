#ifndef RECT_H
#define RECT_H

namespace SparkCell {

	class Rect {
	public:
		Rect(float x, float y, float width, float height);

		float X() const;
		float Y() const;
		float Width() const;
		float Height() const;

		//TODO: convert all of these to a point objects
		float CenterX() const;

		float BottomLeftX() const;
		float BottomLeftY() const;

		float BottomRightX() const;
		float BottomRightY() const;

		float TopLeftX() const;
		float TopLeftY() const;

		float TopRightX() const;
		float TopRightY() const;

		void MoveCenter(float x, float y);

		void MoveTop(float y);
		void MoveBottom(float y);

		void MoveLeft(float y);

	private:
		float x_;
		float y_;
		float width_;
		float height_;
	};

}

#endif // RECT_H
