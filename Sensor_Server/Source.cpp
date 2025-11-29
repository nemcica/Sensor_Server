#include "server/Server.h"
#include "client/Client.h"

int main() {
    
    Server server(5700);
    Client client(5700);

    server.run();
    client.run();

    return 0;
}