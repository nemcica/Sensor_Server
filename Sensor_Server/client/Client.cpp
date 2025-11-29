#pragma once
#include "Client.h"
#include <random>
#include <vector>

Client::Client(unsigned short port) :
	m_io_context(), m_socket(m_io_context), m_server_endpoint(boost::asio::ip::make_address("127.0.0.1"), port) {

}

void Client::connectToServer() {
	boost::system::error_code error;
	

	while (true) {
		m_socket.connect(m_server_endpoint, error);

		if (!error) {
			std::cout << "Connected to server." << std::endl;
			break;
		}
		else {
			std::cerr << "Error occured: " << error.message() << std::endl;
		}

		m_socket.close();
		m_socket = boost::asio::ip::tcp::socket(m_io_context);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
}

void Client::sendMessage(const std::string& msg) {
	std::lock_guard<std::mutex> lock(m_write_mutex);
	
	boost::system::error_code error;

	m_socket.write_some(boost::asio::buffer(msg.data(), msg.size()), error);

	if (error) {
		std::cerr << "Error occured: " << error.message() << std::endl;
	}
}

void Client::run() {
	connectToServer();

	int num = 5;
	std::vector<std::thread> threads;

	for (int i = 1; i <= num; ++i) {
		threads.emplace_back(std::thread(sensorThread, this, i));
	}

	for (auto& t : threads) t.join();

	m_socket.close();
}

void sensorThread(Client* client, int sensorId) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> r_dis(0.0, 100.0);
	std::uniform_int_distribution<> i_dis(100, 2500);

	while (true) {
		float value = r_dis(gen);
		int time = i_dis(gen);

		std::string msg = "SENSOR: " + std::to_string(sensorId) + " VALUE: " + std::to_string(value) + "\n";

		client->sendMessage(msg);

		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}
}