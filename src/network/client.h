//
// Created by beklauter on 02.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_CLIENT_H
#define INC_4GEWINNT_CLIENT_CLIENT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <functional>
#include "host.h"

#pragma comment(lib, "ws2_32.lib")

class client {
private:
    SOCKET clientSocket;
    bool connected;
    std::thread receiveThread;
    std::string serverIP;
    int serverPort;

    void receiveMessages();

public:
    client();
    ~client();

    bool connectToServer(const std::string& ip, int port = 54000);
    void disconnect();
    void sendMessage(const NetworkMessage& msg);
    bool isConnected() const { return connected; }

    // Callbacks
    std::function<void(const NetworkMessage&)> onMessageReceived;
    std::function<void()> onDisconnected;
};

#endif //INC_4GEWINNT_CLIENT_CLIENT_H