#pragma once

#include <string>

class HTTPHandler {
    public:
        std::string handle(const std::string &request) const;
    private:
        std::string& _parse(const std::string &request) const;
        std::string _notImplimented() const;
        std::string _head() const;
        std::string _getDate() const;
};

