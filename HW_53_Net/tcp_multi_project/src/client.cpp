#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/**
 * TCP client for testing the multithreaded server
 * Can simulate multiple concurrent clients
 */
class TCPClient {
private:
    SOCKET socket_;           // Client socket descriptor
    std::string server_ip_;   // Server IP address
    int server_port_;         // Server port number
    WSADATA wsa_data_;        // Winsock data
    
public:
    TCPClient() : socket_(INVALID_SOCKET), server_port_(0) {
        WSAStartup(MAKEWORD(2, 2), &wsa_data_);
    }
    
    ~TCPClient() {
        disconnect();
        WSACleanup();
    }
    
    /**
     * Connect to the TCP server
     * @param ip Server IP address
     * @param port Server port number
     * @return true if connection successful, false otherwise
     */
    bool connectToServer(const std::string& ip, int port) {
        server_ip_ = ip;
        server_port_ = port;
        
        // Create client socket
        socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
        if (connect(socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
            std::cerr << "Connection error to server " << ip << ":" << port 
                      << " (error code: " << WSAGetLastError() << ")" << std::endl;
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            return false;
        }
        
        std::cout << "Successfully connected to server " << ip << ":" << port << std::endl;
        return true;
    }
    
    /**
     * Send message to the server
     * @param message Message to send
     * @return true if send successful, false otherwise
     */
    bool sendMessage(const std::string& message) {
        if (socket_ == INVALID_SOCKET) {
            std::cerr << "Socket not initialized" << std::endl;
            return false;
        }
        
        int bytes_sent = send(socket_, message.c_str(), static_cast<int>(message.length()), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "Message send error: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        std::cout << "Sent " << bytes_sent << " bytes: " << message << std::endl;
        return true;
    }
    
    /**
     * Receive message from the server
     * @return Received message or empty string on error
     */
    std::string receiveMessage() {
        if (socket_ == INVALID_SOCKET) {
            return "";
        }
        
        char buffer[4096];
        int bytes_received = recv(socket_, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Message receive error: " << WSAGetLastError() << std::endl;
            return "";
        } else if (bytes_received == 0) {
            std::cout << "Server closed connection" << std::endl;
            return "";
        }
        
        buffer[bytes_received] = '\0';
        return std::string(buffer);
    }
    
    /**
     * Disconnect from the server
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
 * Test function for single client
 * @param client_name Identifier for this client
 */
void testSingleClient(const std::string& client_name) {
    TCPClient client;
    
    if (!client.connectToServer("127.0.0.1", 8080)) {
        return;
    }
    
    // Send multiple test messages
    std::vector<std::string> test_messages = {
        "Hello server! This is " + client_name,
        "Test message number 2 from " + client_name,
        "Testing multithreaded server functionality",
        "exit"  // Command for graceful shutdown
    };
    
    for (const auto& message : test_messages) {
        if (!client.sendMessage(message)) {
            break;
        }
        
        std::string response = client.receiveMessage();
        if (!response.empty()) {
            std::cout << "[" << client_name << "] Server response:\n" << response << "\n" << std::endl;
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
 * Test function for multiple concurrent clients
 * @param client_count Number of clients to simulate
 */
void testMultipleClients(int client_count) {
    std::cout << "=== Testing " << client_count << " clients ===" << std::endl;
    
    std::vector<std::thread> clients;
    
    for (int i = 1; i <= client_count; ++i) {
        clients.emplace_back([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 100)); // Staggered start
            testSingleClient("Client_" + std::to_string(i));
        });
    }
    
    for (auto& client : clients) {
        client.join();
    }
    
    std::cout << "All clients finished" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== TCP Test Client ===" << std::endl;
    
    if (argc >= 2 && std::string(argv[1]) == "multi") {
        int client_count = 5;
        if (argc >= 3) {
            client_count = std::stoi(argv[2]);
        }
        testMultipleClients(client_count);
    } else {
        // Single client test
        testSingleClient("Test_Client");
    }
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}