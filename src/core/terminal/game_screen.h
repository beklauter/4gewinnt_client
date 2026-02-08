//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_GAME_SCREEN_H
#define INC_4GEWINNT_CLIENT_GAME_SCREEN_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "../../game/gamelogic.h"
#include "../../network/host.h"
#include "../../network/client.h"
#include "../sounds.h"
#include <string>
#include <vector>

using namespace ftxui;

class game_screen {
private:
    ScreenInteractive screen;
    gamelogic game;
    int selectedColumn = 3;
    bool isHost;
    bool isMultiplayer;

    int myPlayerId = -1;
    board::Cell myColor = board::Cell::EMPTY;
    bool waitingForRemote = false;

    host* gameHost;
    client* gameClient;

    std::vector<std::string> chatMessages;
    std::string chatInput;

    Component makeGameBoard();
    Component makeChatBox();
    Component makeGameInfo();
    void handleNetworkMove(int column);
    void sendMove(int column);

public:
    game_screen(host* h = nullptr, client* c = nullptr);
    ~game_screen();

    Component makeUI();
    void loop();
};

#endif //INC_4GEWINNT_CLIENT_GAME_SCREEN_H

