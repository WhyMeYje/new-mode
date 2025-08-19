#ifndef CLIENT_H
#define CLIENT_H

#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HOST 54000
#define BUFFER 4096
#define WLOG "/Users/macbook/new-mode/files/client_log.txt"

class Client {
private:
    int clientSocket_;
    sockaddr_in serverAddress;
    std::ofstream outLog;

    bool setupSocket();
    bool connectClient();
    bool handleClient();
    bool cleanUp();

public:
    Client();
    int run();
    ~Client();
};

#endif //CLIENT_H