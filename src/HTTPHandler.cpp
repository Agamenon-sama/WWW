#include "HTTPHandler.h"

#include <sstream>
#include <chrono>
#include <fstream>
#include <iostream>

HTTPHandler::HTTPHandler(/* args */) {
}

HTTPHandler::~HTTPHandler() {
}

std::string HTTPHandler::handle(const std::string &request) {
    /** This function makes all the necessary calls to
     *  process the http request and return the response
     */
    // _parse(request);

    // Parsing
    std::stringstream reqStream(request);
    std::string line;
    std::getline(reqStream, line);

    std::stringstream requestLine(line);
    std::string method;
    std::getline(requestLine, method, ' ');

    if(method == "GET") {
        // TODO: impliment GET method
        std::string url;
        std::getline(requestLine, url, ' ');
        
        std::string version;
        std::getline(requestLine, version, ' ');
        if(version != "HTTP/1.1\r") { // if the version is neither 1.1 or 1.0
            std::cout << version << "\n";
            std::string response = "HTTP/1.1 505 HTTP Version Not Supported\r\nConnection: close\r\n";
            return response;
        }

        url.insert(0, "web"); // web will contain all the files the server serves
        if(url == "web/") { // if the client doesn't specify a file we send the default one
            url += "index.html";
        }

        // Loading the requested file
        std::fstream targetFile;
        targetFile.open(url, std::ios::in);
        if(!targetFile.is_open()) { // if it doesn't open we handle the error
            std::cerr << "Failed to load the file\n";
            // for now we assume that every time a file doesn't open, it's a bad request
            std::string response = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";
            response += "<html><body><h1>404 Not Found</h1></body></html>";
            return response;
        }
        // If everything is good we load the file
        std::string fileLine;
        std::string fileContent = "";
        while (std::getline(targetFile, fileLine)) {
            fileContent += "\n" + fileLine;
        }
        targetFile.close();

        // Preparing and sending the response
        std::string response = _head();
        response += "Content-Length: " + fileContent.size();
        response += "\r\n" + fileContent;

        return response;
        

    } else if(method == "HEAD") {
        return _head();
    } else {
        return _notImplimented();
    }
}

std::string HTTPHandler::_notImplimented() {
    /** Returns HTTP 501 status (I feel I'm gonna need this a lot) */
    std::string response = "HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n\r\n";
    return response;
}

std::string HTTPHandler::_head() {
    std::string response = "HTTP/1.1 200 OK\r\n\
Connection: close\r\n\
Server: WWW/1.0 (Ubuntu)\r\n\
Date: ";
    response += _getDate() + "\r\n";
    // As a note WWW = "Worst Web server in the World" it's a joke on "World Wide Web"
    return response;
}

std::string HTTPHandler::_getDate() {
    std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = ctime(&rawTime);
    std::string formatedDate = date.substr(0, 3) + ", " + date.substr(8, 3) + date.substr(4, 4) + date.substr(20, 4)
           + " " + date.substr(11, 8);
    return formatedDate;
    // TODO: add time zone
}
