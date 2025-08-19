#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <locale>

#include "client.h"

int main () {
    setlocale(LC_ALL, "ru");

    Client clientA;
    return clientA.run();
}

//=============================================

Client::Client() : clientSocket_(-1) {
    outLog.open(WLOG, std::ios::app);
    if (!outLog.is_open()) {
        outLog.close();
        return;
    }
    outLog << "----ПОДКЛЮЧЕНИЕ КЛИЕНТА----\n";
}

Client::~Client() {
    cleanUp();
}

int Client::run() {
    if (!outLog.is_open()) {
        return -1;
    }

    if (!setupSocket()) { return -1; }
    if (!connectClient()) { return -1; }

    handleClient();
    outLog << "----ЗАВЕРШЕНИЕ ПРОГРАММЫ----\n\n\n";
    return 0;
}

bool Client::setupSocket() {
    clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_ == -1) {
        outLog << "Ошибка: не удалось создать сокет\n";
        return false;
    }
    outLog << "Сокет создан\n";
    
    return true;
}

bool Client::connectClient() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(HOST);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        outLog << "Ошибка: не удалось подключиться к серверу\n";
        return false;
    }
    outLog << "Подключение к серверу установлено!\n";

    std::cout << "Подключение к серверу установлено!\n";
    std::cout << "Теперь вы можете отправлять сообщения\n";
    std::cout << "Команды: 'exit' или 'quit' для выхода\n";

    return true;
}

bool Client::handleClient() {
    std::string message;
    while (true) {
        std::cout << "Клиент: ";
        std::getline(std::cin, message);

        if (message.empty()) {
            std::cout << "Сообщение не может быть пустым\n";
            continue;
        }

        if (send(clientSocket_, message.c_str(), message.length(), 0) < 0) {
            outLog << "Ошибка отправки сообщения\n";
            break;
        }

        if (message == "exit" || message == "quit") {
            send(clientSocket_, message.c_str(), message.length(), 0);
            outLog << "Выход из программы...\n";
            std::cout << "Выход из программы...\n";
            cleanUp();
            break;
        }
    }

    return true;
}

bool Client::cleanUp() {
    if (clientSocket_ != -1) {
        close(clientSocket_);
        clientSocket_ = -1;
    }

    if (outLog.is_open()) {
        outLog.close();
    }

    return true;
}