#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

class TCPServer {
private:
    SOCKET server_fd;
    sockaddr_in address;
    std::vector<std::thread> client_threads;
    std::mutex console_mutex;
    std::mutex threads_mutex;
    std::atomic<bool> running;
    WSADATA wsaData;
    
public:
    TCPServer() : server_fd(INVALID_SOCKET), running(false) {
        memset(&address, 0, sizeof(address));
        
        // Инициализация Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        }
    }
    
    ~TCPServer() {
        stop();
        WSACleanup();
    }
    
    bool start(int port) {
        // Создание сокета
        server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server_fd == INVALID_SOCKET) {
            std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        // Настройка опций сокета
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
            std::cerr << "Socket configuration error: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            return false;
        }
        
        // Настройка адреса
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        // Привязка сокета к адресу
        if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
            std::cerr << "Error binding the socket to the port " << port << ": " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            return false;
        }
        
        // Прослушивание входящих соединений
        if (listen(server_fd, 10) == SOCKET_ERROR) {
            std::cerr << "Listening error: " << WSAGetLastError() << std::endl;
            closesocket(server_fd);
            return false;
        }
        
        running = true;
        std::cout << "The server is running on the port " << port << std::endl;
        std::cout << "Waiting for connections..." << std::endl;
        return true;
    }
    
    void handleClient(SOCKET client_socket, const std::string& client_ip, int client_port) {
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << "[+] Customer processing " << client_ip << ":" << client_port << std::endl;
        }
        
        char buffer[1024];
        
        while (running) {
            // Прием данных от клиента
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received == SOCKET_ERROR) {
                int error = WSAGetLastError();
                if (error != WSAECONNRESET) {
                    std::lock_guard<std::mutex> lock(console_mutex);
                    std::cerr << "[-] Error receiving data from the client " << client_ip 
                              << ": " << error << std::endl;
                }
                break;
            } else if (bytes_received == 0) {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cout << "[-] Client " << client_ip << ":" << client_port << " disconnected" << std::endl;
                break;
            }
            
            buffer[bytes_received] = '\0';
            std::string message(buffer);
            
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cout << "[→] Received from " << client_ip << ":" << client_port 
                          << " (" << bytes_received << " bytes): " << message << std::endl;
            }
            
            // Формирование ответа
            std::string response = "The server has received your message (" + 
                                  std::to_string(bytes_received) + " bytes): " + message;
            
            // Отправка ответа клиенту
            int bytes_sent = send(client_socket, response.c_str(), (int)response.length(), 0);
            if (bytes_sent == SOCKET_ERROR) {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cerr << "[-] Error sending data to the client " << client_ip 
                          << ": " << WSAGetLastError() << std::endl;
                          << ": " << WSAGetLastError() << std::endl;
                break;
            }
            
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cout << "[←] Sent to the client " << client_ip << ":" << client_port 
                          << " (" << bytes_sent << " bytes)" << std::endl;
            }
        }
        
        closesocket(client_socket);
        
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << "[×] Client processing completed " << client_ip << ":" << client_port << std::endl;
        }
    }
    
    void run() {
        while (running) {
            // Принятие входящего соединения
            sockaddr_in client_addr;
            int client_addr_len = sizeof(client_addr);
            
            SOCKET client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
            
            if (client_socket == INVALID_SOCKET) {
                if (running && WSAGetLastError() != WSAEINTR) {
                    std::cerr << "Connection socket error: " << WSAGetLastError() << std::endl;
                }
                continue;
            }
            
            // Получение IP адреса и порта клиента
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            int client_port = ntohs(client_addr.sin_port);
            
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                std::cout << "[+] New connection from " << client_ip << ":" << client_port << std::endl;
            }
            
            // Создание потока для обработки клиента
            {
                std::lock_guard<std::mutex> lock(threads_mutex);
                client_threads.emplace_back(
                    &TCPServer::handleClient, this, 
                    client_socket, std::string(client_ip), client_port
                );
            }
        }
    }
    
    void stop() {
        if (!running) return;
        
        running = false;
        
        // Закрытие серверного сокета для прерывания accept()
        if (server_fd != INVALID_SOCKET) {
            closesocket(server_fd);
            server_fd = INVALID_SOCKET;
        }
        
        // Ожидание завершения всех потоков
        {
            std::lock_guard<std::mutex> lock(threads_mutex);
            for (auto& thread : client_threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
            client_threads.clear();
        }
        
        std::cout << "[!] The server is stopped" << std::endl;
    }
};

// Глобальный указатель для обработки событий
TCPServer* g_server = nullptr;

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        std::cout << "\n[!] A signal has been received Ctrl+C, stopping the server..." << std::endl;
        if (g_server) {
            g_server->stop();
        }
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    TCPServer server;
    g_server = &server;
    
    // Установка обработчика Ctrl+C
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cout << "Can't install handler Ctrl+C" << std::endl;
    }
    
    // Параметры сервера
    int port = 8080;
    
    if (argc >= 2) {
        try {
            port = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << "Invalid port number" << std::endl;
            return 1;
        }
    }
    
    std::cout << "=== TCP Server (Windows) ===" << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "For stop press Ctrl+C" << std::endl;
    
    // Запуск сервера
    if (!server.start(port)) {
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    // Запуск основного цикла сервера
    server.run();
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}