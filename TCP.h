#ifndef FINAL_TCP_H
#define FINAL_TCP_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <arpa/inet.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

struct CircularBuffer {
    char data[BUFFER_SIZE];
    int head = 0;
    int tail = 0;

    bool isEmpty() const { return head == tail; }
    bool isFull() const { return (tail + 1) % BUFFER_SIZE == head; }

    void push(char c) {
        if (!isFull()) {
            data[tail] = c;
            tail = (tail + 1) % BUFFER_SIZE;
        } else {
            std::cerr << "Buffer is full!" << std::endl;
        }
    }

    char pop() {
        if (!isEmpty()) {
            char c = data[head];
            head = (head + 1) % BUFFER_SIZE;
            return c;
        } else {
            std::cerr << "Buffer is empty!" << std::endl;
            return '\0';
        }
    }
};

void handleClient(int client_socket) {
    CircularBuffer buffer;
    char message[BUFFER_SIZE];

    while (true) {
        int bytes_received = recv(client_socket, message, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        for (int i = 0; i < bytes_received; ++i) {
            buffer.push(message[i]);
        }

        std::string ack_message = "Сообщение получено!";
        send(client_socket, ack_message.c_str(), ack_message.length(), 0);
    }

    close(client_socket);
}

int server() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Ошибка создания сокета!" << std::endl;
        return 1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Ошибка привязки сокета!" << std::endl;
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        std::cerr << "Ошибка прослушивания сокета!" << std::endl;
        return 1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    while (true)
    {
        sockaddr_in client_address{};
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size);
        if (client_socket < 0) {
            std::cerr << "Ошибка принятия соединения!" << std::endl;
            continue;
        }

        // Создание потока для обработки клиента
        std::thread client_thread(handleClient, client_socket);
        client_thread.detach();
    }

    close(server_socket);
    return 0;
}

#endif //FINAL_TCP_H
