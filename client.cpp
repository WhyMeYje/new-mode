#include <iostream>
#include <string>
#include <cstring>
#include <locale>

#include "client.h"

int main () {
    setlocale(LC_ALL, "ru");

    int client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ == -1) {
        std::cout << "Ошибка: не удалось создать сокет\n";
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(HOST);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cout << "Ошибка: не удалось подключиться к серверу\n";
        return -1;
    }

    std::cout << "Подключение к серверу установлено!\n";
    std::cout << "Теперь вы можете отправлять сообщения\n";
    std::cout << "Команды: 'exit' или 'quit' для выхода\n\n";

    std::string message;
    std::getline(std::cin, message);

    if (send(client_socket_, message.c_str(), message.length(), 0) < 0) {
        std::cout << "Ошибка отправки сообщения\n";
        return -1;
    }

    if (message == "exit" || message == "quit") {
        send(client_socket_, message.c_str(), message.length(), 0);
        std::cout << "Выход из программы...\n";
        return -1;
    }

    char buffer[BUFFER];
    memset(buffer, 0, BUFFER);
    int bytes_received = recv(client_socket_, buffer, BUFFER - 1, 0);
    if (bytes_received > 0) {
        std::cout << "Ответ сервера: " << std::string(buffer, bytes_received) << "\n\n";
    } 
    else {
        std::cout << "Ошибка получения ответа от сервера\n";
        return -1;
    }

    while (true) {
        std::getline(std::cin, message);

        if (message.empty()) {
            std::cout << "Сообщение не может быть пустым\n";
            continue;
        }

        if (send(client_socket_, message.c_str(), message.length(), 0) < 0) {
            std::cout << "Ошибка отправки сообщения\n";
            break;
        }

        if (message == "exit" || message == "quit") {
            send(client_socket_, message.c_str(), message.length(), 0);
            std::cout << "Выход из программы...\n";
            break;
        }

        char buffer[BUFFER];
        memset(buffer, 0, BUFFER);
        int bytes_received = recv(client_socket_, buffer, BUFFER - 1, 0);
        if (bytes_received > 0) {
            std::cout << "Ответ сервера: " << std::string(buffer, bytes_received) << "\n\n";
        } 
        else {
            std::cout << "Ошибка получения ответа от сервера\n";
            break;
        }
    }

    close(client_socket_);
    return 0;
}