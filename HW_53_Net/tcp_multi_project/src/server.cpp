#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/**
 * TCP Client for testing the server
 */
class TCPClient {
private:
    SOCKET socket_;
    std::string server_ip_;
    int server_port_;
    WSADATA wsa_data_;

public:
    TCPClient() : socket_(INVALID_SOCKET), server_port_(0) {
        WSAStartup(MAKEWORD(2, 2), &wsa_data_);
    }

    ~TCPClient() {
        disconnect();
        WSACleanup();
    }

    /**
     * Connect to TCP server
     */
    bool connectToServer(const std::string& ip, int port) {
        server_ip_ = ip;
        server_port_ = port;

        // Create socket
        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == INVALID_SOCKET) {
            std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
            return false;
        }

        // Configure server address
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid IP address: " << ip << std::endl;
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            return false;
        }

        // Connect to server
        if (connect(socket_, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Connection error to " << ip << ":" << port
                << " (error: " << WSAGetLastError() << ")" << std::endl;
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            return false;
        }

        std::cout << "Connected to server: " << ip << ":" << port << std::endl;
        return true;
    }

    /**
     * Send message to server
     */
    bool sendMessage(const std::string& message) {
        if (socket_ == INVALID_SOCKET) {
            std::cerr << "Socket not connected" << std::endl;
            return false;
        }

        int bytes_sent = send(socket_, message.c_str(), (int)message.length(), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Send error: " << WSAGetLastError() << std::endl;
            return false;
        }

        std::cout << "Sent " << bytes_sent << " bytes: " << message << std::endl;
        return true;
    }

    /**
     * Receive message from server
     */
    std::string receiveMessage() {
        if (socket_ == INVALID_SOCKET) {
            return "";
        }

        char buffer[1024];
        int bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Receive error: " << WSAGetLastError() << std::endl;
            return "";
        }
        else if (bytes_received == 0) {
            std::cout << "Server closed connection" << std::endl;
            return "";
        }

        buffer[bytes_received] = '\0';
        return std::string(buffer);
    }

    /**
     * Disconnect from server
     */
    void disconnect() {
        if (socket_ != INVALID_SOCKET) {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            std::cout << "Disconnected from server" << std::endl;
        }
    }
};

/**
 * Test single client connection
 */
void testSingleClient(const std::string& client_name) {
    TCPClient client;

    std::cout << "[" << client_name << "] Connecting to server..." << std::endl;

    if (!client.connectToServer("127.0.0.1", 8080)) {
        return;
    }

    // Test messages
    std::vector<std::string> messages;
    messages.push_back("Hello from " + client_name);
    messages.push_back("Test message 1");
    messages.push_back("Test message 2");
    messages.push_back("exit");

    for (size_t i = 0; i < messages.size(); ++i) {
        const std::string& message = messages[i];

        if (!client.sendMessage(message)) {
            break;
        }

        std::string response = client.receiveMessage();
        if (!response.empty()) {
            std::cout << "[" << client_name << "] Server response: " << response << std::endl;
        }

        // Small delay between messages
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (message == "exit") {
            break;
        }
    }

    client.disconnect();
}

/**
 * Test multiple concurrent clients
 */
void testMultipleClients(int client_count) {
    std::cout << "=== Testing " << client_count << " concurrent clients ===" << std::endl;

    std::vector<std::thread> clients;

    for (int i = 1; i <= client_count; ++i) {
        std::string client_name = "Client_" + std::to_string(i);
        clients.push_back(std::thread([client_name, i]() {
            // Stagger client starts
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 200));
            testSingleClient(client_name);
            }));
    }

    // Wait for all clients to finish
    for (size_t i = 0; i < clients.size(); ++i) {
        clients[i].join();
    }

    std::cout << "All test clients finished" << std::endl;
}

/**
 * Main function for client
 */
int main(int argc, char* argv[]) {
    std::cout << "=== TCP Test Client ===" << std::endl;

    if (argc >= 2 && std::string(argv[1]) == "multi") {
        int client_count = 3;
        if (argc >= 3) {
            client_count = std::stoi(argv[2]);
        }
        testMultipleClients(client_count);
    }
    else {
        // Single client mode
        testSingleClient("TestClient");
    }

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}