#pragma once
#include <string>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>

enum SensorType
{
	SensorTypeUnknown,
	SensorTypeTemperature,
	SensorTypeSound,
	SensorTypeWeight,
	SensorTypeHumidity,
	SensorTypePressure,
	SensorTypeLightIntensity
};

enum SensorUnit
{
	SensorUnitUnknown,
	SensorUnitDegreesCelsius,
	SensorUnitDecibel,
	SensorUnitKilograms,
	SensorUnitPercent,
	SensorUnitPascal,
	SensorUnitLumen
};

enum SensorPlacement
{
	SensorPlacementInside,
	SensorPlacementOutside
};

typedef boost::bimap<SensorType, std::string> SensorTypeStringType;
const static SensorTypeStringType SensorTypeStrings = boost::assign::list_of< SensorTypeStringType::relation >
	( SensorTypeUnknown, "unknown" )
	( SensorTypeTemperature, "temperature" )
	( SensorTypeSound, "sound" )
	( SensorTypeWeight, "weight" )
	( SensorTypeHumidity, "humidity" )
	( SensorTypePressure, "pressure" )
	( SensorTypeLightIntensity, "light-intensity" );

typedef boost::bimap<SensorUnit, std::string> SensorUnitStringType;
const static SensorUnitStringType SensorUnitStrings = boost::assign::list_of< SensorUnitStringType::relation >
	(SensorUnitUnknown, "unknown")
	(SensorUnitDegreesCelsius, "*C")
	(SensorUnitDecibel, "dB")
	(SensorUnitKilograms, "kg")
	(SensorUnitPercent, "%")
	(SensorUnitPascal, "p")
	(SensorUnitLumen, "lumen");

typedef boost::bimap<SensorPlacement, std::string> SensorPlacementStringType;
const static SensorPlacementStringType SensorPlacementStrings = boost::assign::list_of< SensorPlacementStringType::relation >
	(SensorPlacementInside, "inside")
	(SensorPlacementOutside, "outside");

struct Position
{
	double x;
	double y;
	double z;
};

class Sensor
{
public:
	Sensor();
	virtual ~Sensor();

	virtual SensorType GetSensorType() const = 0;
	virtual SensorUnit GetSensorUnit() const = 0;
	virtual SensorPlacement GetSensorPlacement() const = 0;

	virtual size_t NumValues() const = 0;
	virtual double GetValue(size_t i = 0) const = 0;

	virtual Position GetPosition() const = 0;
};