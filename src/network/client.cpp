//
// Created by beklauter on 02.02.2026.
//

#include "client.h"
#include <iostream>

client::client() : clientSocket(INVALID_SOCKET), connected(false), serverPort(54000) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
}

client::~client() {
    disconnect();
    WSACleanup();
}

bool client::connectToServer(const std::string& ip, int port) {
    serverIP = ip;
    serverPort = port;

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return false;
    }

    connected = true;
    receiveThread = std::thread(&client::receiveMessages, this);

    std::cout << "Connected to server " << ip << ":" << port << std::endl;
    return true;
}

void client::disconnect() {
    if (!connected) return;

    connected = false;

    if (clientSocket != INVALID_SOCKET) {
        shutdown(clientSocket, SD_BOTH);
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }

    if (receiveThread.joinable()) {
        receiveThread.join();
    }

    // Gib dem System Zeit um die Verbindung freizugeben
    Sleep(500);
}

void client::receiveMessages() {
    while (connected) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string received(buffer);

            // Parse message (Format: type|playerId|column|playerColor|data)
            NetworkMessage msg;
            size_t pos = 0;

            // Parse type
            pos = received.find('|');
            msg.type = static_cast<MessageType>(std::stoi(received.substr(0, pos)));
            received = received.substr(pos + 1);

            // Parse playerId
            pos = received.find('|');
            msg.playerId = std::stoi(received.substr(0, pos));
            received = received.substr(pos + 1);

            // Parse column
            pos = received.find('|');
            msg.column = std::stoi(received.substr(0, pos));
            received = received.substr(pos + 1);

            // Parse playerColor
            pos = received.find('|');
            msg.playerColor = std::stoi(received.substr(0, pos));
            msg.data = received.substr(pos + 1);

            if (onMessageReceived) {
                onMessageReceived(msg);
            }
        } else if (bytesReceived == 0) {
            // Connection closed
            std::cout << "Server closed connection" << std::endl;
            connected = false;
            if (onDisconnected) {
                onDisconnected();
            }
            break;
        } else {
            // Error
            std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
            connected = false;
            if (onDisconnected) {
                onDisconnected();
            }
            break;
        }
    }
}

void client::sendMessage(const NetworkMessage& msg) {
    if (!connected) return;

    // Serialisiere die Message
    std::string serialized = std::to_string(static_cast<int>(msg.type)) + "|" +
                            std::to_string(msg.playerId) + "|" +
                            std::to_string(msg.column) + "|" +
                            std::to_string(msg.playerColor) + "|" +
                            msg.data;

    send(clientSocket, serialized.c_str(), static_cast<int>(serialized.length()), 0);
}
