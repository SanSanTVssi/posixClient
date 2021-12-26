//
// Created by aai2002 on 12/25/21.
//

#ifndef CLIENT_OLD_H
#define CLIENT_OLD_H
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
using uint256 = unsigned long;

const uint256 port = 4444;

class logging {
public:
    static void WriteLine(const std::string&& message) {
        std::cout << message << std::endl;
    }

    static void ThrowError(const std::string&& message) {
        throw std::runtime_error(message);
    }

    static void ErrorExit(const std::string&& message, int const code) {
        WriteLine(std::move(message));
        exit(code);
    }
};

void client() {
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    socklen_t addressLength = sizeof(address);
    int clientFD = socket(AF_INET, SOCK_STREAM, 0);
    if(!clientFD) {
        logging::ThrowError("Socket creation failed!");
    }
    if(connect(clientFD, (struct sockaddr*)&address, addressLength)) {
        logging::WriteLine("Client connection failed");
    }
    int readLength;
    auto message = "Hello from client!";
    readLength = send(clientFD, message, strlen(message), 0);
}
#endif //CLIENT_OLD_H
