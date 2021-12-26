//
// Created by aai2002 on 26.12.21.
// * Interface for Client
// * (can be use for all realisation of client
// * but in this proj it use only for posix client)
//

#ifndef CLIENT_ICLIENT_H
#define CLIENT_ICLIENT_H

#include <iostream>

namespace my_std {
    class IClient {
    public:
        virtual bool connect() = 0;
        virtual std::string read() = 0;
        virtual void send(const std::string& message) = 0;
        virtual void sendfile(const std::string& filename) = 0;
        virtual bool disconnect() = 0;
        virtual ~IClient() = default;
    };

}

#endif //CLIENT_ICLIENT_H
