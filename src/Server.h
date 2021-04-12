#pragma once

#include <netinet/in.h>

#include <queue>
#include <thread>
#include <vector>

class Server {
    public:
        Server();
        ~Server();

        bool init();
        void run();

    private:
        int _serverSocket;
        int _clientSocket;
        struct sockaddr_in _serverAddr;
        struct sockaddr_in _clientAddr;
        std::queue<int*> _requestQueue;
        std::vector<std::thread> _threadPool;
        size_t _threadPoolSize;

        // void _handleRequest();
};

void handleRequest(std::queue<int*> requestQueue );
void threadFunction(std::queue<int*> requestQueue);