//
//
// Created by aai2002 on 12/25/21.
// Library for work with unix(posix) server-client
//
//

#ifndef SERVER_BASE_H
#define SERVER_BASE_H

#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <thread>
#include <sys/sendfile.h>
#include "logging.h"

namespace my_std {
    using uint256_t = unsigned long;
    using filedesc_t = int;
    const uint256_t defaultPort = 4444;
    using buff_t = char;
    const buff_t defVal = 0;
    const uint256_t DEFAULT_MAX_BUFFER_SIZE = 1024;

    /**
     * only for unix(posix)
     * Function for get sizeof file
     * @param file - unix file for close
     * @return size - sizeof file
     */
    off_t filesize(filedesc_t file);
    /**
     * only for unix(posix)
     * Function close file. If closing failed
     * print message in console
     * @param file - unix file for close
     */
    void closefile(filedesc_t file);

    /**
     * Struct for buffer while reading from socket/file
     */
    struct Buffer {
    private:
        buff_t * _buffer;
        std::size_t _size;
        buff_t _def_val;
    public:
        Buffer(std::size_t size, buff_t value, buff_t def_val = defVal)
                : _buffer(new buff_t[size] {0}), _size(size), _def_val(def_val) { }
        ~Buffer() { delete[] _buffer; }

        /**
         * Set values as default in buffer
         */
        [[maybe_unused]] void zeroingBuffer() { memset(_buffer, defVal, _size); }
        std::size_t size() { return _size; }
        buff_t * getBuf() { return _buffer; }
        /**
         * Equal to strlen for c-string but with custom def-value
         */
        [[maybe_unused]] std::size_t fullness() {
            std::size_t j = 0;
            for (std::size_t i = 0; i < _size; ++i) {
                if (_buffer[i] == _def_val) { break; }
                j++;
            }
            return j;
        }
        buff_t getDefVal() { return _def_val; }
    };

    /**
     * Struct for work with posix(unix) address struct for posix(unix) socket
     */
    struct address {
    private:
        sockaddr_in _address{};
        socklen_t addressLength;

    public:
        explicit address(
                in_addr_t ip = INADDR_ANY,
                in_port_t port = defaultPort,
                sa_family_t family = AF_INET)
                : addressLength(sizeof(_address))
        {
            _address.sin_addr.s_addr = ip;
            _address.sin_port = htons(port);
            _address.sin_family = family;
        }

        struct sockaddr* getSockaddrPtr() {
            return (struct sockaddr*)&_address;
        }

        socklen_t* getLenPtr() { return &addressLength; }

        [[nodiscard]] socklen_t length() const { return addressLength; }
    };
}

#endif //SERVER_BASE_H
