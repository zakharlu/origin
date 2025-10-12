#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

class TCPClient {
private:
    SOCKET sockfd;
    sockaddr_in server_addr;
    WSADATA wsaData;
    
public:
    TCPClient() : sockfd(INVALID_SOCKET) {
        memset(&server_addr, 0, sizeof(server_addr));
        
        // Инициализация Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        }
    }
    
    ~TCPClient() {
        disconnect();
        WSACleanup();
    }
    
    bool connectToServer(const std::string& ip, int port) {
        // Создание сокета
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "Error create socket: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        // Настройка адреса сервера
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Wrong IP address: " << ip << std::endl;
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            return false;
        }
        
        // Подключение к серверу
        if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Server connection error " << ip << ":" << port 
                      << ": " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            return false;
        }
        
        std::cout << "Successfully connected to the server " << ip << ":" << port << std::endl;
        return true;
    }
    
    bool sendMessage(const std::string& message) {
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "The socket is not initialized" << std::endl;
            return false;
        }
        
        int bytes_sent = send(sockfd, message.c_str(), (int)message.length(), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Error sending the message: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        std::cout << "Sent " << bytes_sent << " bytes: " << message << std::endl;
        return true;
    }
    
    std::string receiveMessage(int timeout_seconds = 5) {
        if (sockfd == INVALID_SOCKET) {
            return "";
        }
        
        // Установка таймаута на прием данных
        DWORD timeout = timeout_seconds * 1000;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        
        char buffer[1024];
        int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAETIMEDOUT) {
                std::cerr << "Timeout for waiting for a response from the server" << std::endl;
            } else {
                std::cerr << "Error receiving the message: " << error << std::endl;
            }
            return "";
        } else if (bytes_received == 0) {
            std::cout << "The server has closed the connection" << std::endl;
            return "";
        }
        
        buffer[bytes_received] = '\0';
        std::string response(buffer);
        
        std::cout << "Received " << bytes_received << " bytes: " << response << std::endl;
        return response;
    }
    
    void disconnect() {
        if (sockfd != INVALID_SOCKET) {
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            std::cout << "Disconnected from the server" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    TCPClient client;
    
    // Параметры подключения
    std::string server_ip = "127.0.0.1";
    int server_port = 8080;
    std::string message = "Hello, server! This is a test message from a Windows client.";
    
    if (argc >= 2) {
        server_ip = argv[1];
    }
    if (argc >= 3) {
        try {
            server_port = std::stoi(argv[2]);
        } catch (...) {
            std::cerr << "Invalid port number" << std::endl;
            return 1;
        }
    }
    if (argc >= 4) {
        message = argv[3];
    }
    
    std::cout << "=== TCP Client (Windows) ===" << std::endl;
    std::cout << "Connecting to " << server_ip << ":" << server_port << std::endl;
    
    // Подключение к серверу
    if (!client.connectToServer(server_ip, server_port)) {
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    // Отправка тестовой строки
    if (!client.sendMessage(message)) {
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    // Прием ответа от сервера
    std::string response = client.receiveMessage();
    if (response.empty()) {
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    // Закрытие соединения
    client.disconnect();
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}