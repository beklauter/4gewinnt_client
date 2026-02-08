//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_PLAY_SCREEN_H
#define INC_4GEWINNT_CLIENT_PLAY_SCREEN_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "../sounds.h"
#include "game_screen.h"
#include "../../network/host.h"
#include "../../network/client.h"

using namespace ftxui;

class play_screen {
private:
    ScreenInteractive* screen = nullptr;
    int selectedMode = 0;
    std::string hostName = "Mein Spiel";
    std::string joinIP = "127.0.0.1";
    bool inLobby = false;

    host* gameHost = nullptr;
    client* gameClient = nullptr;

public:
    play_screen();
    ~play_screen();

    void loop();
    void startGame();
};

#endif //INC_4GEWINNT_CLIENT_PLAY_SCREEN_H