#include "SensorServerInfo.hxx"

SensorServerInfo::SensorServerInfo()
{

}

SensorServerInfo::SensorServerInfo(SensorType _type, SensorUnit _unit, SensorPlacement _placement, size_t _num_values, const Position& _position)
{
	type = _type;
	unit = _unit;
	placement = _placement;
	num_values = _num_values;
	position = _position;
}

SensorServerInfo::~SensorServerInfo()
{

}

SensorType SensorServerInfo::GetSensorType() const
{
	return type;
}

SensorUnit SensorServerInfo::GetSensorUnit() const
{
	return unit;
}

SensorPlacement SensorServerInfo::GetSensorPlacement() const
{
	return placement;
}

size_t SensorServerInfo::NumValues() const
{
	return num_values;
}

double SensorServerInfo::GetValue(size_t i) const
{
	return 0.0;
}

Position SensorServerInfo::GetPosition() const
{
	return position;
}