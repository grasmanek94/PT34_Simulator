#include <json-develop/src/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

#include <DeviceSetup.hxx>
#include <SensorServerInfo.hxx>

typedef websocketpp::client<websocketpp::config::asio> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef client::message_ptr message_ptr;
using json = nlohmann::json;

void on_open(client* s, websocketpp::connection_hdl hdl)
{

	/*std::cout << "on_open called with hdl: " << hdl.lock().get()
		<< std::endl;*/

	/*try
	{
		s->send(hdl, (const void*)"hello", 6, websocketpp::frame::opcode::TEXT);
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}*/
}

void on_close(client* s, websocketpp::connection_hdl hdl) 
{
	/*std::cout << "on_close called with hdl: " << hdl.lock().get()
		<< std::endl;*/
}

SensorServerInfo temperature(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementInside, 1, Position{9.0, 10.0, -10.6});
SensorServerInfo sound(SensorTypeSound, SensorUnitDecibel, SensorPlacementInside, 1, Position{ 8.0, 20.0, -10.5 });
SensorServerInfo weight(SensorTypeWeight, SensorUnitKilograms, SensorPlacementInside, 1, Position{ 7.0, 30.0, -10.4 });
SensorServerInfo humidity(SensorTypeHumidity, SensorUnitPercent, SensorPlacementInside, 1, Position{ 6.0, 40.0, -10.3 });
SensorServerInfo pressure(SensorTypePressure, SensorUnitPascal, SensorPlacementInside, 1, Position{ 5.0, 50.0, -10.2 });
SensorServerInfo lightintensity(SensorTypeLightIntensity, SensorUnitLumen, SensorPlacementInside, 1, Position{ 4.0, 60.0, -10.1 });

// Define a callback to handle incoming messages
void on_message(client* s, websocketpp::connection_hdl hdl, message_ptr msg) 
{
	json j = json::parse(msg->get_payload().c_str());

	auto device_setup = j.find("GetDeviceSetup");
	if (device_setup != j.end()) {
		
		DeviceSetup setup;
		setup.SetSerial("CLIENT0001");
		setup.AddCapability(DeviceCapabilities_3g | DeviceCapabilities_bluetooth30 | DeviceCapabilities_ethernet100m);
		setup.AddSensor(&temperature);
		setup.AddSensor(&temperature);
		setup.AddSensor(&temperature);
		setup.AddSensor(&sound);
		setup.AddSensor(&weight);
		setup.AddSensor(&humidity);
		setup.AddSensor(&humidity);
		setup.AddSensor(&lightintensity);

		setup.ParseRequestJson(device_setup->dump());
		
		std::string response(setup.GetResponseJson());

		try 
		{
			s->send(hdl, response.c_str(), response.length() + 1, websocketpp::frame::opcode::TEXT);
		}
		catch (const websocketpp::lib::error_code& e) 
		{
			std::cout << "Echo failed because: " << e
				<< "(" << e.message() << ")" << std::endl;
		}
	}

	/*std::cout << "on_message called with hdl: " << hdl.lock().get()
		<< " and message: " << msg->get_payload()
		<< std::endl;

	// check for a special command to instruct the server to stop listening so
	// it can be cleanly exited.
	if (msg->get_payload() == "stop-listening") {
		s->stop_listening();
		return;
	}

	try {
		s->send(hdl, msg->get_payload(), msg->get_opcode());
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}*/
}

int main() 
{
	// Create a server endpoint
	client echo_client;

	try 
	{
		// Set logging settings
		echo_client.set_access_channels(websocketpp::log::alevel::all);
		echo_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		echo_client.init_asio();

		// Register our message handler
		echo_client.set_message_handler(bind(&on_message, &echo_client, ::_1, ::_2));

		echo_client.set_open_handler(bind(&on_open, &echo_client, ::_1));
		echo_client.set_close_handler(bind(&on_close, &echo_client, ::_1));

		websocketpp::lib::error_code ec;
		std::string uri = "ws://localhost:9002";

		client::connection_ptr con = echo_client.get_connection(uri, ec);
		if (ec) 
		{
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return 0;
		}

		echo_client.connect(con);

		// Start the ASIO io_service run loop
		echo_client.run();
	}
	catch (websocketpp::exception const & e) 
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::exception const & e)
	{
		std::cout << "other exception: " << e.what() << std::endl;
	}
}