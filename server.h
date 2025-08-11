#ifndef SERVER_H
#define SERVER_H

#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define HOST 54000
#define BUFFER 4096
#define WLOG "/Users/macbook/new-mode/files/server_log.txt"
#define WHISTORY "/Users/macbook/new-mode/files/history.txt"

class Server {
private:
    int socket_;
    int client_fd;
    sockaddr_in serverAddress;
    std::ofstream outLog;
    std::ofstream outHistory;

    bool setupSocket();
    bool bindSocket();
    bool listenSocket();
    bool acceptClient();
    bool handleClient();
    bool cleanUp();

public:
    Server();
    int run();
    ~Server();
};

#endif //SERVER_H  