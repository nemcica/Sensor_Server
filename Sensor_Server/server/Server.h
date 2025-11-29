#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <vector>

struct Reading {
	int sensor_id;
	float sensor_value;
	std::chrono::steady_clock::time_point time_stamp;

	Reading(int sensor_id, float sensor_value, std::chrono::steady_clock::time_point time_stamp): 
		sensor_id(sensor_id), sensor_value(sensor_value), time_stamp(time_stamp) {}
};

class Server {
private:
	boost::asio::io_context m_io_context;
	boost::asio::ip::tcp::acceptor m_acceptor;
	unsigned short m_port;

	std::vector<Reading> readings;
	std::mutex m_read_mutex;
	std::atomic<bool> m_running = true;

	void acceptClient();
	void handleClient(boost::asio::ip::tcp::socket socket);
	void aggregator();
	Reading parseData(std::string& data);

public:
	Server(unsigned short port);
	~Server();
	void run();
	void stop();
};

