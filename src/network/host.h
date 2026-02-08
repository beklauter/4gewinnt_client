//
// Created by beklauter on 02.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_HOST_H
#define INC_4GEWINNT_CLIENT_HOST_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

enum class MessageType {
    PLAYER_JOIN,
    PLAYER_LEAVE,
    PLAYER_ASSIGNED,      // Neue Message wenn Spieler Rolle zugewiesen wird
    GAME_START,
    MOVE,
    MOVE_RECEIVED,        // Bestätigung dass Zug empfangen wurde
    CHAT,
    GAME_STATE,
    WIN,
    DRAW
};

struct NetworkMessage {
    MessageType type;
    int playerId;
    int column;
    int playerColor;      // 1 = PLAYER1 (Rot), 2 = PLAYER2 (Gelb)
    std::string data;
};

class host {
private:
    SOCKET serverSocket;
    std::vector<SOCKET> clients;
    std::mutex clientsMutex;
    bool running;
    std::thread acceptThread;
    std::thread receiveThread;
    int port;
    std::string hostName;
    bool gameStarted = false;
    int nextPlayerId = 0;

    void acceptClients();
    void receiveMessages();
    void handleMessage(SOCKET clientSocket, const NetworkMessage& msg);

public:
    host(int serverPort = 54000);
    ~host();

    bool startServer(const std::string& name);
    void stopServer();
    void startGame();
    void broadcastMessage(const NetworkMessage& msg);
    void sendToClient(SOCKET clientSocket, const NetworkMessage& msg);
    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] int getClientCount() const { return static_cast<int>(clients.size()); }
    [[nodiscard]] bool isGameStarted() const { return gameStarted; }
    std::string getLocalIP();

    // Callback für empfangene Nachrichten
    std::function<void(int, const NetworkMessage&)> onMessageReceived;
    std::function<void(int)> onPlayerJoined;
    std::function<void(int)> onPlayerLeft;
};

#endif //INC_4GEWINNT_CLIENT_HOST_H