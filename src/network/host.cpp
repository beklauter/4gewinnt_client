//
// Created by beklauter on 02.02.2026.
//

#include "host.h"
#include <iostream>

host::host(int serverPort) : serverSocket(INVALID_SOCKET), running(false), port(serverPort) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
}

host::~host() {
    stopServer();
    WSACleanup();
}

std::string host::getLocalIP() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        return "Unknown";
    }

    struct addrinfo hints, *info;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &info) != 0) {
        return "Unknown";
    }

    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)info->ai_addr;
    inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ip, INET_ADDRSTRLEN);

    freeaddrinfo(info);
    return std::string(ip);
}

bool host::startServer(const std::string& name) {
    hostName = name;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    // Erlaube Socket-Wiederverwendung um TIME_WAIT zu vermeiden
    int reuseAddr = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddr, sizeof(reuseAddr)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        return false;
    }

    sockaddr_in serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        return false;
    }

    running = true;
    acceptThread = std::thread(&host::acceptClients, this);
    receiveThread = std::thread(&host::receiveMessages, this);

    std::cout << "Server started on " << getLocalIP() << ":" << port << std::endl;
    return true;
}

void host::stopServer() {
    if (!running) return;

    running = false;

    // Close all client connections
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (SOCKET client : clients) {
            shutdown(client, SD_BOTH);
            closesocket(client);
        }
        clients.clear();
    }

    if (serverSocket != INVALID_SOCKET) {
        shutdown(serverSocket, SD_BOTH);
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    if (acceptThread.joinable()) acceptThread.join();
    if (receiveThread.joinable()) receiveThread.join();

    // Gib dem System Zeit um die Ports freizugeben
    Sleep(500);
}

void host::startGame() {
    gameStarted = true;

    NetworkMessage startMsg;
    startMsg.type = MessageType::GAME_START;
    broadcastMessage(startMsg);
}

void host::acceptClients() {
    while (running) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            if (running) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            }
            break;
        }

        {
            std::lock_guard<std::mutex> lock(clientsMutex);

            if (clients.size() >= 2) {
                closesocket(clientSocket);
                continue;
            }

            clients.push_back(clientSocket);
            int playerId = nextPlayerId++;

            NetworkMessage assignMsg;
            assignMsg.type = MessageType::PLAYER_ASSIGNED;
            assignMsg.playerId = playerId;
            assignMsg.playerColor = (playerId == 0) ? 1 : 2;

            sendToClient(clientSocket, assignMsg);

            if (onPlayerJoined) {
                onPlayerJoined(playerId);
            }
        }
    }
}

void host::receiveMessages() {
    while (running) {
        std::lock_guard<std::mutex> lock(clientsMutex);

        for (auto it = clients.begin(); it != clients.end();) {
            SOCKET clientSocket = *it;

            // Set non-blocking mode for checking
            u_long mode = 1;
            ioctlsocket(clientSocket, FIONBIO, &mode);

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
                    onMessageReceived(std::distance(clients.begin(), it), msg);
                }

                if (msg.type == MessageType::MOVE) {
                    for (size_t i = 0; i < clients.size(); ++i) {
                        if (i != static_cast<size_t>(std::distance(clients.begin(), it))) {
                            sendToClient(clients[i], msg);
                        }
                    }
                }

                ++it;
            } else if (bytesReceived == 0 || WSAGetLastError() != WSAEWOULDBLOCK) {
                if (onPlayerLeft) {
                    onPlayerLeft(std::distance(clients.begin(), it));
                }
                closesocket(clientSocket);
                it = clients.erase(it);
            } else {
                ++it;
            }
        }

        Sleep(10);
    }
}

void host::broadcastMessage(const NetworkMessage& msg) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    // Serialisiere die Message (einfaches Format: type|playerId|column|playerColor|data)
    std::string serialized = std::to_string(static_cast<int>(msg.type)) + "|" +
                            std::to_string(msg.playerId) + "|" +
                            std::to_string(msg.column) + "|" +
                            std::to_string(msg.playerColor) + "|" +
                            msg.data;

    for (SOCKET client : clients) {
        send(client, serialized.c_str(), static_cast<int>(serialized.length()), 0);
    }
}

void host::sendToClient(SOCKET clientSocket, const NetworkMessage& msg) {
    std::string serialized = std::to_string(static_cast<int>(msg.type)) + "|" +
                            std::to_string(msg.playerId) + "|" +
                            std::to_string(msg.column) + "|" +
                            std::to_string(msg.playerColor) + "|" +
                            msg.data;

    send(clientSocket, serialized.c_str(), static_cast<int>(serialized.length()), 0);
}

void host::handleMessage(SOCKET clientSocket, const NetworkMessage& msg) {
    switch (msg.type) {
        case MessageType::MOVE:
            broadcastMessage(msg);
            break;
        case MessageType::CHAT:
            broadcastMessage(msg);
            break;
        default:
            break;
    }
}
