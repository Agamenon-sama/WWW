#pragma once

#include <string>

class HTTPHandler {
    public:
        HTTPHandler(/* args */);
        ~HTTPHandler();

        std::string handle(const std::string &request);
    private:
        std::string& _parse(const std::string &request);
        std::string _notImplimented();
        std::string _head();
        std::string _getDate();
};

