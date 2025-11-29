#include "Server.h"

Server::Server(unsigned short port): 
	m_io_context(), 
	m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), 
	m_port(port) {

	std::cout << "Server initialized on port: " << port << std::endl;
}

Server::~Server() {
	stop();
}

void Server::acceptClient() {
	std::cout << "Waiting for client to connect..." << std::endl;

	while (m_running) {
		boost::asio::ip::tcp::socket client_socket(m_io_context);
		try {
			m_acceptor.accept(client_socket);
		}
		catch (boost::system::system_error& e) {
			std::cerr << "The acceptor was closed!" << std::endl;
		}
		
		std::cout << "Client connected!" << std::endl;

		std::thread(&Server::handleClient, this, std::move(client_socket)).detach();
	}
}

void Server::handleClient(boost::asio::ip::tcp::socket socket) {
	boost::system::error_code error;
	boost::asio::streambuf buffer;

	while (m_running) {

		boost::asio::read_until(socket, buffer, '\n', error);

		if (error == boost::asio::error::eof) {
			std::cout << "Client disconnected!" << std::endl;
			break;
		}
		else if (error) {
			std::cerr << "Error occured: " << error.message() << std::endl;
			break;
		}

		std::istream is(&buffer);
		std::string line;
		std::getline(is, line);

		{
			std::lock_guard<std::mutex> lock(m_read_mutex);
			readings.emplace_back(parseData(line));
		}
	}
}

void Server::aggregator() {

	while (m_running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));

		float sum = 0;
		float min, max, avg;
		int count;

		{
			std::lock_guard<std::mutex> lock(m_read_mutex);

			if (readings.empty()) continue;

			min = readings[0].sensor_value;
			max = readings[0].sensor_value;
			count = readings.size();

			for (auto& reading : readings) {
				sum += reading.sensor_value;
				if (reading.sensor_value < min) min = reading.sensor_value;
				if (reading.sensor_value > max) max = reading.sensor_value;
			}
	
			readings.clear();
		}

		avg = sum / count;
		std::cout << "[STATS]: avg = " << avg << ", min = " << min << ", max = " << max << ", count = " << count << std::endl;
	}
}

Reading Server::parseData(std::string& data) {
	std::stringstream ss(data);
	std::string denominator;

	int sensor_id;
	float reading_value;
	std::chrono::steady_clock::time_point time_stamp = std::chrono::steady_clock::now();

	ss >> denominator;
	ss >> sensor_id;
	ss >> denominator;
	ss >> reading_value;

	return Reading(sensor_id, reading_value, time_stamp);
}


void Server::run() {

	std::thread t(&Server::aggregator, this);

	acceptClient();

	t.join();
}

void Server::stop() {
	m_running = false;

	m_acceptor.close();
}