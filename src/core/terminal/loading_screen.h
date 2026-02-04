//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_LOADINGSCREEN_H
#define INC_4GEWINNT_CLIENT_LOADINGSCREEN_H


#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "../states.h"
#include "../../game/game.h"
#include <thread>
#include <chrono>
#include "windows.h"
#include "menu_screen.h"

using namespace ftxui;

class loading_screen {
private:
    ScreenInteractive screen;
    float progress = 0.0f;
    bool done = false;

    Component makeUI();

public:
    loading_screen();
    void loop();
    void addLog(const std::string& msg);
};



#endif //INC_4GEWINNT_CLIENT_LOADINGSCREEN_H