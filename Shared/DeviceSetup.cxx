#include <json-develop/src/json.hpp>

#include "DeviceSetup.hxx"

using json = nlohmann::json;

DeviceSetup::DeviceSetup()
{

}

DeviceSetup::~DeviceSetup()
{

}

std::string DeviceSetup::GetRequestJson() const
{
	json j
	{
		{ "GetDeviceSetup", {} }
	};

	return j.dump();
}

void DeviceSetup::ParseRequestJson(const std::string& json)
{

}

std::string DeviceSetup::GetResponseJson() const
{
	json j
	{
		{ "GetDeviceSetup",
			{"serial", serial}
		}
	};

	for (auto& sensor_type : sensors)
	{
		if (sensor_type.second.size() > 0)
		{
			j["GetDeviceSetup"]["devices"].push_back({ std::to_string(sensor_type.first), sensor_type.second.size() });
		}
	}

	return j.dump();
}

void DeviceSetup::ParseResponseJson(const std::string& json)
{

}

void DeviceSetup::SetSerial(const std::string& _serial)
{
	serial = _serial;
}

std::string DeviceSetup::GetSerial() const
{
	return serial;
}

bool DeviceSetup::AddSensor(Sensor* sensor)
{
	if (sensor == NULL)
	{
		return false;
	}

	sensors[sensor->GetSensorType()].push_back(sensor);
	return true;
}

void DeviceSetup::AddCapability(size_t capability)
{
	capabilities |= capability;
}

void DeviceSetup::RemoveCapability(size_t capability)
{
	capabilities &= ~(capability);
}

bool DeviceSetup::HasCapability(size_t capability)
{
	return (capabilities & capability) != 0;
}

size_t DeviceSetup::GetCapabilities()
{
	return (size_t)capabilities;
}