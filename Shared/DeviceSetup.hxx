#pragma once

#include <string>
#include <vector>
#include <map>

#include "Sensor.hxx"

enum DeviceCapabilities
{
	DeviceCapabilities_None = 0,
	DeviceCapabilities_ethernet10m = 1 << 0,
	DeviceCapabilities_ethernet100m = 1 << 1,
	DeviceCapabilities_ethernet1000m = 1 << 2,
	DeviceCapabilities_ethernet10gbps = 1 << 3,
	DeviceCapabilities_3g = 1 << 4,
	DeviceCapabilities_4g = 1 << 5,
	DeviceCapabilities_wifi24ghz80211b = 1 << 6,
	DeviceCapabilities_wifi24ghz80211g = 1 << 7,
	DeviceCapabilities_wifi24ghz80211n = 1 << 8,
	DeviceCapabilities_wifi50ghz80211a = 1 << 9,
	DeviceCapabilities_wifi50ghz80211 = 1 << 10,
	DeviceCapabilities_wifi50ghz80211j = 1 << 11,
	DeviceCapabilities_wifi50ghz80211n = 1 << 12,
	DeviceCapabilities_wifi50ghz80211ac = 1 << 13,
	DeviceCapabilities_bluetooth20 = 1 << 14,
	DeviceCapabilities_bluetooth21 = 1 << 15,
	DeviceCapabilities_bluetooth30 = 1 << 16,
	DeviceCapabilities_bluetooth31 = 1 << 17,
	DeviceCapabilities_bluetooth40 = 1 << 18,
	DeviceCapabilities_bluetooth41 = 1 << 19,
	DeviceCapabilities_bluetooth42 = 1 << 20,
	DeviceCapabilities_bluetooth5 = 1 << 21
};

class DeviceSetup
{
public:
	using sensors_container_t = std::vector<Sensor*>;
	using sensors_list_t = std::map<SensorType, sensors_container_t>;
private:
	std::string serial;
	sensors_list_t sensors;
	size_t capabilities;
public:
	DeviceSetup();
	~DeviceSetup();

	std::string GetRequestJson() const;
	std::string GetResponseJson() const;
	void ParseRequestJson(const std::string& json);
	void ParseResponseJson(const std::string& json);

	void SetSerial(const std::string& _serial);
	std::string GetSerial() const;

	bool AddSensor(Sensor* sensor);

	void AddCapability(size_t capability);
	void RemoveCapability(size_t capability);
	bool HasCapability(size_t capability);
	size_t GetCapabilities();
};