#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Server.h"

int main() {
    Server server;
    if(!server.init()) {
        std::cerr << "Server couldn't initilize\nExiting\n";
        return 1;
    }
    std::cout << "The server is ON\n";
    server.run();
    
    return 0;
}