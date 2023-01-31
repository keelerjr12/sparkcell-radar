#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <PdkServices.h>

namespace SparkCell {

	class Aircraft
	{
	public:
		Aircraft(const CComPtr<P3D::IBaseObjectV400>& obj);

		float lat() const;
		float lon() const;
		float alt() const;

		float pitch() const;
		float bank() const;
		float heading() const;

		float velocityX() const;
		float velocityY() const;
		float velocityZ() const;

		float airspeed() const;

	private:
		CComPtr<P3D::IBaseObjectV400> mObj;
	};

}

#endif // AIRCRAFT_H
