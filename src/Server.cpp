#include "Server.h"
#include "HTTPHandler.h"

#include <iostream>
#include <mutex>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

std::mutex queueMutex;

Server::Server() {
    // Initilizing everything to 0
    _serverSocket = 0;
    _clientSocket = 0;
    memset(&_serverAddr, 0, sizeof(struct sockaddr_in));
    memset(&_clientAddr, 0, sizeof(struct sockaddr_in));
    _threadPoolSize = 8;

    _threadPool.resize(_threadPoolSize); // Set the thread pool size
    for(size_t i = 0; i < _threadPoolSize; i++) {
        // _threadPool[i] = std::thread(handleRequest, _requestQueue);
        _threadPool[i] = std::thread(threadFunction, _requestQueue);
    }
}

Server::~Server() {
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
    _serverAddr.sin_port = htons(8080);
    _serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Binding the socket to the address
    if(bind(_serverSocket, (struct sockaddr*) &_serverAddr, sizeof(_serverAddr)) == -1) {
        std::cerr << "Failed to bind the socket to address\n";
        return false;
    }

    // Setting the socket to passive
    listen(_serverSocket, 5);

    return true;
}


void Server::run() {
    while(1) {
        // Accept the connection
        socklen_t peerAddrSize = sizeof(_clientAddr);
        _clientSocket = accept(_serverSocket, (struct sockaddr*) &_clientAddr, &peerAddrSize);

        // Displaying the IP of the peer app
        char peerIP[INET_ADDRSTRLEN] = {0};
        if(inet_ntop(AF_INET, &_clientAddr.sin_addr, peerIP, sizeof(peerIP))) {
            std::cout << "Accepted connection with " << peerIP << "\n";
        } else {
            std::cout << "Failed to get the IP of the client\n";
        }

        int *pclient = new int;
        *pclient = _clientSocket;
        queueMutex.lock();
        _requestQueue.push(pclient);
        queueMutex.unlock();
        std::cout << "Pushed request to the queue\n";

        // handleRequest(_requestQueue);
        
        // Close connection
        // close(_clientSocket);
    }
}


void threadFunction(std::queue<int*> requestQueue) {
    while (1) {
        // queueMutex.lock();
        if(!requestQueue.empty()){
            // queueMutex.unlock();
            handleRequest(requestQueue);
        }
    
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20ms);
    }
}

void handleRequest(std::queue<int*> requestQueue) {
    // while(1) {
        // if(!requestQueue.empty()){
            // Poping the client socket from the queue
            queueMutex.lock();
            int *clientSocket = requestQueue.front();
            requestQueue.pop();
            queueMutex.unlock();

            // Receiving request and processing it
            char request[2 * 1024];
            recv(*clientSocket, request, sizeof(request), 0);
            std::cout << "Client Request : \n" << request << "\n";

            HTTPHandler requestHandler;
            // std::cout << "Created handler\n";
            std::string reply = requestHandler.handle(request);
            std::cout << reply << "\n";

            // Sending the reply
            send(*clientSocket, reply.c_str(), reply.size(), 0);

            // Closing connection
            close(*clientSocket);
        // }

    // }
}