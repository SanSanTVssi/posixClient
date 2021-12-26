#include "Client.h"
#include <thread>
using namespace my_std;

void ClientProcessing(IClient * client, std::string message) {
    client->connect();
    client->send(message);
    logging::WriteLine(client->read());
    client->disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
    posixClient client;
    ClientProcessing(&client, "Hello");
    posixClient client2;
    ClientProcessing(&client2, "Привет");
    return 0;
}
