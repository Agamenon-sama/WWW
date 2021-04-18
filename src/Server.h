#pragma once

#include <netinet/in.h>

#include <queue>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

class Server {
    public:
        Server();
        ~Server();

        bool init();
        void run();

    private:
        int _serverSocket;
        int _clientSocket;
        uint16_t _portNumber;
        struct sockaddr_in _serverAddr;
        struct sockaddr_in _clientAddr;
        std::queue<int> _requestQueue;
        std::vector<std::thread> _threadPool;
        size_t _threadPoolSize;
        std::mutex _queueMutex;
        std::condition_variable _cv;

        void _handleRequest();
        bool _loadConfig();
};

void handleRequest(std::queue<int*> requestQueue );
// void threadFunction(std::queue<int*> &requestQueue);