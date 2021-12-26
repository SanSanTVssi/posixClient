//
// Created by aai2002 on 12/25/21.
//

#include "Client.h"

my_std::posixClient::posixClient(in_addr_t ip, int connectionType, in_port_t port, sa_family_t family, int protocol,
                                 std::size_t bufferSize) : _buffer(bufferSize, 0), _isConnected(false)
{
    _clientFD = socket(family, connectionType, protocol);
    if(!_clientFD) {
        throw std::runtime_error("Socket creation failed!");
    }
    logging::WriteTimedLine("The client started up");
}

my_std::posixClient::~posixClient() {
    logging::WriteTimedLine("The client has finished working");

    if (_isConnected) {
        closefile(_clientFD);
        _isConnected = false;
    }
}

bool my_std::posixClient::connect() {
    if(::connect(_clientFD, _address.getSockaddrPtr(), _address.length())) {
        logging::WriteLineForcibly("Client connection failed");
        _isConnected = false;
    }
    else {
        logging::WriteTimedLine("Client connection succeed");
        _isConnected = true;
    }
    return _isConnected;
}

std::string my_std::posixClient::read() {
    if(!_isConnected) return {};
    std::size_t size;
    std::size_t readLen = ::read(_clientFD, &size, sizeof(size));
    if (readLen == 0) { return {}; }
    if(size > _buffer.size()) {
        throw std::runtime_error("Message read failed. Message too big");
    }
    //char * buffer = new char[size + 1] {0};
    readLen = ::read(_clientFD, _buffer.getBuf(), size);
    if (readLen == 0) { return {}; }
    std::string temp (_buffer.getBuf());
    logging::WriteTimedLine("Read message:\t" + temp);
    _buffer.zeroingBuffer();
    return temp;
}

void my_std::posixClient::send(const std::string &message) {
    if(!_isConnected) return;
    send(message, 0);
}

void my_std::posixClient::send(const std::string &message, int flags) {
    auto size = message.size();
    ::send(_clientFD, &size, sizeof(size), flags);
    ::send(_clientFD, message.c_str(), size, flags);
    logging::WriteTimedLine("Send message:\t" + message);
}

void my_std::posixClient::sendingFileErrorProcessing(my_std::filedesc_t file) {
    closefile(file);
    throw std::runtime_error("Send file failed!");
}

void my_std::posixClient::sendfile(const std::string &filename) {
    if(!_isConnected) return;
    sendfile(filename, 0);
}

void my_std::posixClient::sendfile(const std::string &filename, int flags) {
    if(!_isConnected) return;
    logging::WriteTimedLine("Start sending file " + filename);
    filedesc_t file = open(filename.c_str(), O_RDONLY);
    if (!file) {
        logging::WriteLineForcibly("Error while opening file!");
        return;
    }
    sendfile(file, flags);
}

void my_std::posixClient::sendfile(my_std::filedesc_t file, int flags) {
    if(!_isConnected) return;
    off_t fsize = filesize(file);
    off_t sendFsize = ::send(_clientFD, &fsize, 1*sizeof(off_t), flags);
    if (!sendFsize) {
        sendingFileErrorProcessing(file);
    }
    const auto bufsize = static_cast<off_t>(_buffer.size()*sizeof(_buffer.getDefVal()));

    off_t i = 0;
    while(i < fsize) {
        off_t sendFileSize = ::sendfile(_clientFD, file, &i, bufsize);
        if (!sendFileSize) {
            sendingFileErrorProcessing(file);
        }
    }
    closefile(file);
    logging::WriteTimedLine("Finish sending file");
}

bool my_std::posixClient::disconnect() {
    long flagDisconnect = 0xD;
    ::send(_clientFD, &flagDisconnect, sizeof(flagDisconnect), 0);
    closefile(_clientFD);
    _isConnected = false;
    return false;
}
