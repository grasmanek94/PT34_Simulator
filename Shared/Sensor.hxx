#pragma once

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
	SensorUnitKilograms,
	SensorUnitDecibel,
	SensorUnitPercent,
	SensorUnitPascal,
	SensorUnitLumen
};

enum SensorPlacement
{
	SensorPlacementInside,
	SensorPlacementOutside
};

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