#ifndef FINAL_CLIENT_H
#define FINAL_CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const char* SERVER_IP = "127.0.0.1";

void client()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Ошибка создания сокета!" << std::endl;
        return;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Ошибка подключения к серверу!" << std::endl;
        return;
    }

    std::cout << "Подключено к серверу!" << std::endl;

    while (true) {
        std::string message;
        std::cout << "Введите сообщение: ";
        std::getline(std::cin, message);

        send(client_socket, message.c_str(), message.length(), 0);

        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received <= 0) {
            std::cerr << "Ошибка получения данных от сервера!" << std::endl;
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << "Получено от сервера: " << buffer << std::endl;
    }

    close(client_socket);
    return;
}

#endif //FINAL_CLIENT_H
