cmake_minimum_required(VERSION 3.21)
project(Client)

set(CMAKE_CXX_STANDARD 23)

add_executable(Client main.cpp
        Client.cpp
        Client.h
        base.cpp
        base.h
        logging.cpp
        logging.h IClient.h)
