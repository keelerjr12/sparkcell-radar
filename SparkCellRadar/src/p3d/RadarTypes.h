#ifndef RADAR_TYPES_H
#define RADAR_TYPES_H

#include <gauges.h>

struct PropertyTable
{
    PCSTRINGZ szPropertyName;
    PCSTRINGZ szUnitsName;
    ENUM units;
};

struct AircraftPropertyTable
{
    LPCWSTR szPropertyName;
    LPCWSTR units;
};

#endif // RADAR_TYPES_H