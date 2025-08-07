#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <locale>

#include "client.h"

int main () {
    setlocale(LC_ALL, "ru");

    std::ofstream outLog;
    outLog.open(WLOG, std::ios::app);
    if (!outLog.is_open()) {
        outLog.close();
        exit(EXIT_FAILURE);
    }
    outLog << "----ПОДКЛЮЧЕНИЕ КЛИЕНТА----\n";

    int client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ == -1) {
        outLog << "Ошибка: не удалось создать сокет\n";
        return -1;
    }
    outLog << "Сокет создан\n";

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(HOST);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        outLog << "Ошибка: не удалось подключиться к серверу\n";
        return -1;
    }
    outLog << "Подключение к серверу установлено!\n";

    std::cout << "Подключение к серверу установлено!\n";
    std::cout << "Теперь вы можете отправлять сообщения\n";
    std::cout << "Команды: 'exit' или 'quit' для выхода\n";

    std::string message;
    while (true) {
        std::cout << "Клиент: ";
        std::getline(std::cin, message);

        if (message.empty()) {
            std::cout << "Сообщение не может быть пустым\n";
            continue;
        }

        if (send(client_socket_, message.c_str(), message.length(), 0) < 0) {
            outLog << "Ошибка отправки сообщения\n";
            break;
        }

        if (message == "exit" || message == "quit") {
            send(client_socket_, message.c_str(), message.length(), 0);
            outLog << "Выход из программы...\n";
            std::cout << "Выход из программы...\n";
            close(client_socket_);
            break;
        }
    }

    outLog << "----ЗАВЕРШЕНИЕ ПРОГРАММЫ----\n\n\n";
    outLog.close();
    close(client_socket_);
    return 0;
}