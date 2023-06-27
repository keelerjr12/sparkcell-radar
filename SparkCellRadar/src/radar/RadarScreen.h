#ifndef RADAR_SCREEN_H
#define RADAR_SCREEN_H

#include "../ui/Label.h"
#include <string>
#include <vector>

#include <gauges.h>

namespace SparkCell {

	class Radar;
	class VirtualDisplay;

	class RadarScreen {
	public:
		
		RadarScreen(VirtualDisplay* vd, const Radar* const radar);

		void Render();

	private:
		void Setup();

		VirtualDisplay* vd_;
		const Radar* const radar_;

		std::vector<std::wstring> top_lbl_strs_ { L"CRM", L"RWS", L"NORM", L"OVRD", L"CNTL" };
		std::vector<std::wstring> bottom_lbl_strs_ { L"SWAP", L"FCR", L"HSD", L"WPN", L"DCLT" };

		std::vector<Label> top_lbls_;
		std::vector<Label> bottom_lbls_;

		SparkCell::Label rng_lbl_;
	};

}

#endif