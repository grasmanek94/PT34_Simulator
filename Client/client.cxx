#include <json-develop/src/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

#include <DeviceSetup.hxx>
#include <DummySensor.hxx>

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

// Define a callback to handle incoming messages
void on_message(client* s, websocketpp::connection_hdl hdl, message_ptr msg) 
{
	json j = json::parse(msg->get_payload().c_str());

	auto device_setup = j.find("GetDeviceSetup");
	if (device_setup != j.end()) {
		
		DeviceSetup setup(
			"CLIENT0001",
			DevCapabilities_3g | DevCapabilities_bluetooth30 | DevCapabilities_ethernet100m,
			{
				new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementInside, 1, -100.0, 200.0, Position{ 3.0, 10.0, -10.6 }),
				new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementInside, 1, -100.0, 200.0, Position{ 4.0, 10.0, -10.5 }),
				new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 5.0, 10.0, -10.4 }),
				new DummySensor(SensorTypeHumidity, SensorUnitPercent, SensorPlacementInside, 1, -100.0, 200.0, Position{ 6.0, 10.0, -10.3 }),
				new DummySensor(SensorTypeHumidity, SensorUnitPercent, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 7.0, 10.0, -10.2 }),
				new DummySensor(SensorTypeSound, SensorUnitDecibel, SensorPlacementInside, 1, -100.0, 200.0, Position{ 8.0, 10.0, -10.1 }),
				new DummySensor(SensorTypeSound, SensorUnitDecibel, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 9.0, 10.0, -10.0 })
			});

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
	DeviceSetup setup(
		"CLIENT0001",
		DevCapabilities_3g | DevCapabilities_bluetooth30 | DevCapabilities_ethernet100m,
		{
			new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementInside, 1, -100.0, 200.0, Position{ 3.0, 10.0, -10.6 }),
			new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementInside, 1, -100.0, 200.0, Position{ 4.0, 10.0, -10.5 }),
			new DummySensor(SensorTypeTemperature, SensorUnitDegreesCelsius, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 5.0, 10.0, -10.4 }),
			new DummySensor(SensorTypeHumidity, SensorUnitPercent, SensorPlacementInside, 1, -100.0, 200.0, Position{ 6.0, 10.0, -10.3 }),
			new DummySensor(SensorTypeHumidity, SensorUnitPercent, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 7.0, 10.0, -10.2 }),
			new DummySensor(SensorTypeSound, SensorUnitDecibel, SensorPlacementInside, 1, -100.0, 200.0, Position{ 8.0, 10.0, -10.1 }),
			new DummySensor(SensorTypeSound, SensorUnitDecibel, SensorPlacementOutside, 1, -100.0, 200.0, Position{ 9.0, 10.0, -10.0 })
		});

	//setup.ParseRequestJson(device_setup->dump());

	std::string response(setup.GetResponseJson());
	setup.ParseResponseJson(response);
	std::cout << std::endl << response << std::endl;
	return 0;
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