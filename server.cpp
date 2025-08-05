#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "server.h"

int main() {
    std::ofstream outLog;
    outLog.open(WLOG, std::ios::app);
    if (!outLog.is_open()) {
        outLog.close();
        return -1;
    }
    outLog << "----ЗАПУСК ПРОГРАММЫ----\n";

    std::ofstream outHistory;
    outHistory.open(WHISTORY, std::ios::app);
    if (!outHistory.is_open()) {
        outHistory.close();
        return -1;
    }

    int socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1) {
        outLog << "Ошибка: не создан сокет\n";
        exit(EXIT_FAILURE);
    }
    outLog << "Сокет создан\n";

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(HOST);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    std::memset(serverAddress.sin_zero, 0, sizeof(serverAddress.sin_zero));

    int b_result = bind(socket_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (b_result < 0) {
        outLog << "Ошибка: Сокет не смог привязаться к адресу и порту\n";
        exit(EXIT_FAILURE);
    }
    outLog << "Сокет привязан к адресу и порту\n";

    int l_result = listen(socket_, SOMAXCONN);
    if (l_result < 0) {
        outLog << "Ошибка: Сокет не перешел в режим прослушивания\n";
        exit(EXIT_FAILURE);
    }
    outLog << "Сокет перешел в режим прослушивания\n";

    socklen_t addr_len = sizeof(serverAddress);
    int client_fd = accept(socket_, (struct sockaddr*)&serverAddress, &addr_len);
    if (client_fd < 0) {
        outLog << "Ошибка: клиент не подключился\n";
        exit(EXIT_FAILURE);
    }
    outLog << "Клиент подключился\n";

    while (true) {
        char buffer[BUFFER];
        memset(buffer, 0, BUFFER);
        int bytes_received = recv(client_fd, buffer, BUFFER - 1, 0);
        if (bytes_received <= 0) {
            outLog << "Клиент отключился\n";
            break;
        }

        std::string client_message(buffer, bytes_received);
        outLog << "Получено сообщение\n";
        outHistory << client_message << std::endl;

        if (client_message.empty()) {
            continue;
        }

        if (client_message == "quit" || client_message == "exit") {
            outLog << "Клиент завершил чат\n";
            break;
        }

        std::string response = client_message;
        int bytes_sent = send(client_fd, response.c_str(), response.length(), 0);
        if (bytes_sent < 0) {
            outLog << "Ошибка отправки клиенту\n";
            break;
        }
        outLog << "Отправлено клиенту: " << response << "\n";
        std::cout << "Клиент: " << client_message << "\n";
    }

    outLog << "----ЗАВЕРШЕНИЕ ПРОГРАММЫ----\n\n\n";
    close(client_fd);
    close(socket_);
    outLog.close();
    outHistory.close();
    return 0;
}