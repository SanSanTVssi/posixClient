//
// Created by aai2002 on 12/25/21.
//

#ifndef SERVER_LOGGING_H
#define SERVER_LOGGING_H

#include <iostream>
#include <chrono>
#include <ctime>

class logging {
private:
    static bool needLogging;
    static std::string time_now() {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timeStr = std::string(std::ctime(&currentTime));
        timeStr.pop_back();
        return timeStr;
    }
public:
    template<class string = std::string>
    static void WriteLine(const string message) {
        if (needLogging) {
            std::cout << message << std::endl;
        }
    }

    template<class string = std::string>
    static void WriteLineForcibly(const string message) {
        std::cout << message << std::endl;
    }

    template<class string = std::string>
    static void ErrorExit(const string message, int const code) {
        WriteLineForcibly(std::move(message));
        exit(code);
    }

    template<class string = std::string>
    static void WriteTimedLine(const string logString) {
        if (needLogging) {
            std::cout << "[" << time_now() << "]: ";
            WriteLine(logString);
        }
    }
};


#endif //SERVER_LOGGING_H
