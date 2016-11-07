#pragma once

#include "Sensor.hxx"

class SensorServerInfo: public Sensor
{
private:
	SensorType type;
	SensorUnit unit;
	SensorPlacement placement;
	size_t num_values;
	Position position;

public:
	SensorServerInfo();
	SensorServerInfo(SensorType _type, SensorUnit _unit, SensorPlacement _placement, size_t _num_values, Position& _position);
	virtual ~SensorServerInfo();

	virtual SensorType GetSensorType() const;
	virtual SensorUnit GetSensorUnit() const;
	virtual SensorPlacement GetSensorPlacement() const;

	virtual size_t NumValues() const;
	virtual double GetValue(size_t i = 0) const;

	virtual Position GetPosition() const;
};