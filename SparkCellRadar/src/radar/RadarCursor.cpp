#include "RadarCursor.h"
#include <algorithm>
#include <functional>

namespace SparkCell {

	static constexpr float DEFAULT_HORZ_SLEW_RATE = .6667f;
	static constexpr float DEFAULT_VERT_SLEW_RATE = .03056f;
	static constexpr int UPDATES_PER_SECOND = 18;

	RadarCursor::RadarCursor(int selected_rng, int max_rng, int min_rng, int max_az, std::function<int()> scope_up_cb, std::function<int()> scope_down_cb) : 
		selected_rng(selected_rng), max_rng(max_rng), min_rng(min_rng), max_az(max_az), m_cursor_az(0.f), m_cursor_rng(selected_rng / 2.f),
		scope_up_cb(std::move(scope_up_cb)), scope_down_cb(std::move(scope_down_cb)) { }

	void RadarCursor::Update() {
		m_cursor_az -= m_x_inc * (60.0f / UPDATES_PER_SECOND);
		m_cursor_rng += selected_rng * m_y_inc;

		m_x_inc = 0;
		m_y_inc = 0;

		CheckBounds();
	}

	void RadarCursor::CheckBounds() {
		m_cursor_az = std::min(m_cursor_az, static_cast<float>(max_az));
		m_cursor_az = std::max(m_cursor_az, -static_cast<float>(max_az));

		m_cursor_rng = std::min(m_cursor_rng, static_cast<float>(max_rng));
		m_cursor_rng = std::max(m_cursor_rng, 0.f);

		const auto selected_rng = static_cast<float>(this->selected_rng);
		const auto upper_rng_bound = selected_rng * .95;
		const auto lower_rng_bound = selected_rng * .05;

		if ((m_cursor_rng <= lower_rng_bound) && (this->selected_rng > min_rng)) {
			this->selected_rng = scope_down_cb();
			m_cursor_rng = this->selected_rng / 2.f;
		} else if ((m_cursor_rng >= upper_rng_bound) && (this->selected_rng < max_rng)) {
			this->selected_rng = scope_up_cb();
			m_cursor_rng = this->selected_rng / 2.f;
		}
	}

	float RadarCursor::Azimuth() const {
		return m_cursor_az;
	}

	float RadarCursor::Range() const {
		return m_cursor_rng;
	}

	void RadarCursor::SlewLeft() {
		m_x_inc = DEFAULT_HORZ_SLEW_RATE;
	}

	void RadarCursor::SlewRight() {
		m_x_inc = -DEFAULT_HORZ_SLEW_RATE;
	}

	void RadarCursor::SlewUp() {
		m_y_inc = DEFAULT_VERT_SLEW_RATE;
	}

	void RadarCursor::SlewDown() {
		m_y_inc = -DEFAULT_VERT_SLEW_RATE;
	}

	bool RadarCursor::IsNear(float az, float rng) const {
		const auto NUM_DEGS = 3.0f;
		const auto NUM_MILES = 1.0f;
		const auto ang_diff = std::abs(az);

		if (ang_diff < NUM_DEGS && abs(m_cursor_rng - rng) < NUM_MILES) {
			return true;
		}

		return false;
	}

}
