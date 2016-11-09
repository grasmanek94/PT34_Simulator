#include <unordered_map>

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
			{ 
				{"serial", serial}
			}
		}
	};

	std::unordered_map<std::string, int> c_umap{};

	for (auto& sensor_type : sensors)
	{
		if (sensor_type.second.size() > 0)
		{
			c_umap[SensorTypeStrings.left.at(sensor_type.first)] = sensor_type.second.size();
		}
	}

	j.find("GetDeviceSetup")->push_back({ "devices", c_umap });
	std::cout << j;

	return j.dump();
}

void DeviceSetup::ParseResponseJson(const std::string& _json)
{
	const json j = json::parse(_json.c_str());
	auto device_setup = j.find("GetDeviceSetup");
	if (device_setup != j.end())
	{
		auto serial_found = device_setup->find("serial");
		if (serial_found != device_setup->end())
		{
			serial = (*serial_found).get<std::string>();
		}
		else
		{
			serial = "UNKNOWN";
		}

		auto devices_found = device_setup->find("devices");
		sensors.clear();
		if (devices_found != device_setup->end())
		{
			serial = (*serial_found).get<std::string>();
		}
		else
		{
			serial = "UNKNOWN";
		}
	}
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