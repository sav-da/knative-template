#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    const int port = 8080;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Ошибка при привязке сокета");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка при прослушивании");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Сервер запущен на порту " << port << std::endl;

    while (true) {
        // Принятие входящего соединения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t*)&addrlen)) < 0) {
            perror("Ошибка при принятии соединения");
            continue;
        }

        // Чтение запроса
        char buffer[30000] = {0};
        read(new_socket, buffer, 30000);
        std::cout << "Получен запрос:\n" << buffer << std::endl;

        // Отправка ответа
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "Hello, World!";
        write(new_socket, response.c_str(), response.size());
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
