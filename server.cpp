#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <errno.h>

#include "server.h"

int main() {
    setlocale(LC_ALL, "ru");
    Server server;
    return server.run();
}

//=============================================

Server::Server() : socket_(-1), client_fd(-1) {
    outLog.open(WLOG, std::ios::app);
    if (!outLog.is_open()) {
        outLog.close();
        return;
    }
    outLog << "----ЗАПУСК ПРОГРАММЫ----\n";

    outHistory.open(WHISTORY, std::ios::app);
    if (!outHistory.is_open()) {
        outHistory.close();
        return;
    }
}

Server::~Server() {
    cleanUp();
}

int Server::run() {
    if (!outLog.is_open() || !outHistory.is_open()) {
        return -1;
    }

    if (!setupSocket()) { return -1; }
    if (!bindSocket()) { return -1; }
    if (!listenSocket()) { return -1; }
    if (!acceptClient()) { return -1; }

    handleClient();
    outLog << "----ЗАВЕРШЕНИЕ ПРОГРАММЫ----\n\n\n";
    return 0;
}

bool Server::setupSocket() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        outLog << "Ошибка: не удалось создать сокет\n";
        return false;
    }
    outLog << "Сокет создан\n";

    return true;
}

bool Server::bindSocket() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(HOST);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    std::memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));

    int b_result = bind(socket_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (b_result < 0) {
        outLog << "Ошибка: Сокет не смог привязаться к адресу и порту\n";
        return false;
    }
    outLog << "Сокет привязан к адресу и порту\n";

    return true;
}

bool Server::listenSocket() {
    int l_result = listen(socket_, SOMAXCONN);
    if (l_result < 0) {
        outLog << "Ошибка: Сокет не перешел в режим прослушивания\n";
        return false;
    }
    outLog << "Сокет перешел в режим прослушивания\n";

    return true;
}

bool Server::acceptClient() {
    socklen_t addr_len = sizeof(serverAddress);
    client_fd = accept(socket_, (struct sockaddr*)&serverAddress, &addr_len);
    if (client_fd < 0) {
        outLog << "Ошибка: клиент не подключился\n";
        exit(EXIT_FAILURE);
    }
    outLog << "Клиент подключился\n";

    return true;
}

bool Server::handleClient() {
    while (true) {
        char buffer[BUFFER];
        memset(buffer, 0, BUFFER);
        int bytes_received = recv(client_fd, buffer, BUFFER - 1, 0);
        if (bytes_received < 0) {
            int err = errno;
            outLog << "Ошибка recv: " << strerror(err) << " (" << err << ")\n";
            break;
        }
        if (bytes_received == 0) {
            outLog << "Клиент отключился\n";
            break;
        }
    
        std::string client_message(buffer, bytes_received);
        if (!client_message.empty() && client_message.back() == '\n') {
            client_message.pop_back();
        }

        outLog << "Получено сообщение\n";
        outHistory << client_message << std::endl;
    
        if (client_message.empty()) {
            continue;
        }
    
        if (client_message == "quit\n" || client_message == "exit\n") {
            outLog << "Клиент завершил чат\n";
            bytes_received = 0;
        }
    
        std::cout << "Клиент: " << client_message << "\n";
    }

    return true;
}

bool Server::cleanUp() {
    if (client_fd != -1) {
        close(client_fd);
        client_fd = -1;
    }
    if (socket_ != -1) {
        close(socket_);
        socket_ = -1;
    }

    if (outLog.is_open()) {
        outLog.close();
    }
    if (outHistory.is_open()) {
        outHistory.close();
    }

    return true;
}