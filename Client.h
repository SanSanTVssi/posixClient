//
// Created by aai2002 on 12/25/21.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "base.h"
#include "IClient.h"

namespace my_std {
    /**
     * Client implementation for work in unix(posix) systems
     */
    class posixClient final : public IClient {
    private:
        address _address;
        filedesc_t _clientFD;
        bool _isConnected;
        Buffer _buffer;
    public:
        explicit posixClient(in_addr_t ip = INADDR_ANY,
                    int connectionType = SOCK_STREAM,
                    in_port_t port = defaultPort,
                    sa_family_t family = AF_INET,
                    int protocol = 0,
                    std::size_t bufferSize = DEFAULT_MAX_BUFFER_SIZE);

        bool is_connected() const { return _isConnected; }

        posixClient(const posixClient& other) = delete;
        posixClient& operator=(const posixClient& other) = delete;

        ~posixClient() override;

        /**
         * Connect to server
         * @return status of connection
         */
        bool connect() final;
        bool disconnect() final;
        /**
        * Read message from server
        * @return message
        */
        std::string read() final;
        /**
         * Send message to server
         * @param message - message to send
         */
        void send(const std::string& message) final;
        void send(const std::string& message, int flags);

    private:
        static void sendingFileErrorProcessing
        (filedesc_t file);
    public:

        /**
         * Send file to server
         * @param filename - name of file
         */
        void sendfile(const std::string& filename) final;
        void sendfile(const std::string& filename, int flags);
        void sendfile(filedesc_t file, int flags);
    };
}


#endif //CLIENT_CLIENT_H
