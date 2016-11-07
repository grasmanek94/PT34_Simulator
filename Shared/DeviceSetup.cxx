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
	json j;
	j["GetDeviceSetup"];
	return j;
}

void DeviceSetup::ParseResponseJson(const std::string& json)
{

}
