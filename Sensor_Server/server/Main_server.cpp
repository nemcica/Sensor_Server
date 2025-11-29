#include "Server.h"

int main() {
	Server server(5700);
	
	std::thread serverThread([&server]() { server.run(); });

	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();

	server.stop();
	serverThread.join();
	
	return 0;
}