#ifndef RADAR_CURSOR_H
#define RADAR_CURSOR_H

#include <functional>

namespace SparkCell {

	class RadarCursor {
	public:

		RadarCursor(int selected_rng, int max_rng, int min_rng, int max_az, std::function<int()> scope_up_cb, std::function<int()> scope_down_cb);

		void Update();

		float Azimuth() const;
		float Range() const;

		void SlewLeft();
		void SlewRight();
		void SlewUp();
		void SlewDown();

		bool IsNear(float az, float rng) const;

	private:
		void CheckBounds();

		int selected_rng;
		int max_rng;
		int min_rng;

		int max_az;

		float m_cursor_az = 0.f;
		float m_cursor_rng = 0.f;

		float m_x_inc = 0.f;
		float m_y_inc = 0.f;

		std::function<int()> scope_up_cb;
		std::function<int()> scope_down_cb;
	};

}
#endif // RADAR_CURSOR_H
