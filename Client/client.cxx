#include <json-develop/src/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

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

	try
	{
		s->send(hdl, (const void*)"hello", 6, websocketpp::frame::opcode::TEXT);
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}
}

void on_close(client* s, websocketpp::connection_hdl hdl) {
	/*std::cout << "on_close called with hdl: " << hdl.lock().get()
		<< std::endl;*/
}

// Define a callback to handle incoming messages
void on_message(client* s, websocketpp::connection_hdl hdl, message_ptr msg) 
{
	json j = json::parse(msg->get_payload().c_str());
	
	for (auto& element : j) 
	{
		std::cout << "----------------" << std::endl << element << std::endl << "----------------" << std::endl;
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