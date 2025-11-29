#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

class Client {
private:
	boost::asio::io_context m_io_context;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::endpoint m_server_endpoint;

	std::mutex m_write_mutex;

	void connectToServer();

public:
	Client(unsigned short port);
	void sendMessage(const std::string& msg);
	void run();
};

void sensorThread(Client* client, int sensorId);