#include "Aircraft.h"
#include <Pdk.h>

namespace SparkCell {

	Aircraft::Aircraft(const CComPtr<P3D::IBaseObjectV400>& obj) : mObj(obj)  {

	}

	void Aircraft::Update(const CComPtr<P3D::IBaseObjectV400>& obj) {
		mObj = obj;
	}

	int Aircraft::id() const {
		return mObj->GetId();
	}

	float Aircraft::lat() const {
		auto lat = 0.0;
		mObj->GetProperty(L"PLANE LATITUDE", L"Degrees", lat);

		return lat;
	}

	float Aircraft::lon() const {
		auto lon = 0.0;
		mObj->GetProperty(L"PLANE LONGITUDE", L"Degrees", lon);

		return lon;
	}

	float Aircraft::alt() const {
		auto alt = 0.0;
		mObj->GetProperty(L"PLANE ALTITUDE", L"Feet", alt);

		return alt;
	}

	float Aircraft::pitch() const {
		auto pitch = 0.0;
		mObj->GetProperty(L"PLANE PITCH DEGREES", L"Degrees", pitch);

		return pitch;
	}

	float Aircraft::bank() const {
		auto bank = 0.0;
		mObj->GetProperty(L"PLANE BANK DEGREES", L"Degrees", bank);

		return bank;
	}

	float Aircraft::heading() const {
		auto heading = 0.0;
		mObj->GetProperty(L"PLANE HEADING DEGREES TRUE", L"Degrees", heading);

		return heading;
	}

	float Aircraft::velocityX() const {
		auto vx = 0.0;
		mObj->GetProperty(L"VELOCITY WORLD X", L"Knots", vx);

		return vx;
	}

	float Aircraft::velocityY() const {
		auto vy = 0.0;
		mObj->GetProperty(L"VELOCITY WORLD Y", L"Knots", vy);

		return vy;
	}

	float Aircraft::velocityZ() const {
		auto vz = 0.0;
		mObj->GetProperty(L"VELOCITY WORLD Z", L"Knots", vz);

		return vz;
	}

	float Aircraft::airspeed() const {
		return sqrt(velocityX() * velocityX() + velocityY() * velocityY() + velocityZ() * velocityZ());
	}

}
