#include "Server.h"
#include "HTTPHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>



Server::Server() {
    // Initilizing everything to the default settings
    _serverSocket = 0;
    _clientSocket = 0;
    memset(&_serverAddr, 0, sizeof(struct sockaddr_in));
    memset(&_clientAddr, 0, sizeof(struct sockaddr_in));
    _threadPoolSize = 8;
    _portNumber = 8080;
    _backlogSize = 16;

    if(!_loadConfig()) {
        std::cerr << "\e[33mWarning : The server will start with default configuration\n\e[0m";
    }
    std::cout << "Port number : " << _portNumber << "\n";
    std::cout << "Backlog size : " << _backlogSize << "\n";

    _threadPool.resize(_threadPoolSize); // Set the thread pool size
    for(size_t i = 0; i < _threadPoolSize; i++) {
        _threadPool[i] = std::thread([this](){_handleRequest();});
    }
}

bool Server::init() {
    /** Initilizes the server by opening the socket
     *  and setting up the necessary data
     */
    // Creating socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket == -1) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    // Set server address
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(_portNumber);
    _serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Binding the socket to the address
    if(bind(_serverSocket, (struct sockaddr*) &_serverAddr, sizeof(_serverAddr)) == -1) {
        std::cerr << "Failed to bind the socket to address\n";
        return false;
    }

    // Setting the socket to passive
    listen(_serverSocket, _backlogSize);

    return true;
}


void Server::run() {
    while(1) {
        // Accept the connection
        socklen_t peerAddrSize = sizeof(_clientAddr);
        _clientSocket = accept(_serverSocket, (struct sockaddr*) &_clientAddr, &peerAddrSize);
        time_t acceptTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string acceptTime_s = ctime(&acceptTime);
        std::cout << acceptTime_s.substr(11, 8) << " :: ";

        // Displaying the IP of the peer app
        char peerIP[INET_ADDRSTRLEN] = {0};
        if(inet_ntop(AF_INET, &_clientAddr.sin_addr, peerIP, sizeof(peerIP))) {
            std::cout << "Accepted connection with " << peerIP << "\n";
        } else {
            std::cout << "Failed to get the IP of the client\n";
        }

        _queueMutex.lock();
        _requestQueue.push(_clientSocket);
        _cv.notify_one();
        _queueMutex.unlock();
        std::cout << "Pushed request to the queue\n";
    }
}


bool Server::_loadConfig() {
    /** Loads the configuration file and sets
     * the appropriate properties accordingly
     */

    std::fstream configFile;
    configFile.open("config", std::ios::in);
    if(!configFile.is_open()) {
        std::cerr << "\e[33mWarning : Failed to load configuration file\n\e[0m";

        return false;
    }

    int lineNum = 1;
    std::string line;
    std::string key;
    std::string value;
    while(getline(configFile, line)) {
        std::stringstream ssline(line);
        std::getline(ssline, key, '=');
        std::getline(ssline, value);

        if(key[0] == '#') {
            lineNum++;
            continue;
        } else if(key == "pool_size") {
            try {
                _threadPoolSize = std::stoi(value);
            } catch(std::exception &e) {
                std::cerr << "\e[31mError : Can't set the pool size " << e.what() << "\e[0m";
            }
        } else if(key == "port_number") {
            try {
                _portNumber = std::stoi(value);
            } catch(std::exception &e) {
                std::cerr << "\e[31mError : Can't set the port number " << e.what() << "\e[0m";
            }
        } else if(key == "backlog_size") {
            try {
                _backlogSize = std::stoi(value);
            } catch(std::exception &e) {
                std::cerr << "\e[31mError : Can't set the backlog size " << e.what() << "\e[0m";
            } 
        } else {
            std::cerr << "\e[33mWarning : Invalid configuration key at line " << lineNum << "\n\e[0m";
        }
        lineNum++;
    }
    configFile.close();

    return true;
}

void Server::_handleRequest() {
    /** Reads from the request queue and creats and HTTPHandler
     * to handle the request
     */

    std::unique_lock<std::mutex> ulock(_queueMutex, std::defer_lock);
    int clientSocket = -1;

    while(1) {
        // Poping the client socket from the queue
        ulock.lock();
        _cv.wait(ulock, [this](){ return !_requestQueue.empty();}); // if predicate is true we execute
        clientSocket = _requestQueue.front();                       // else we wait for notify
        _requestQueue.pop();
        ulock.unlock();

        // Receiving request and processing it
        char request[2 * 1024];
        recv(clientSocket, request, sizeof(request), 0);
        std::cout << "Client Request : \n" << request << "\n";

        HTTPHandler requestHandler;
        // std::cout << "Created handler\n";
        std::string reply = requestHandler.handle(request);
        std::cout << reply << "\n";

        // Sending the reply
        send(clientSocket, reply.c_str(), reply.size(), 0);

        // Closing connection
        close(clientSocket);
    }
}