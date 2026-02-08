#ifndef INC_4GEWINNT_CLIENT_MENU_SCREEN_H
#define INC_4GEWINNT_CLIENT_MENU_SCREEN_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ftxui/component/component.hpp>
#include "../states.h"
#include "ftxui/component/screen_interactive.hpp"
#include <conio.h>
#include <iostream>
#include "../sounds.h"

class menu_screen;
class play_screen;

using namespace ftxui;

extern menu_screen* g_menu_screen;

class menu_screen {
    ftxui::ScreenInteractive* screen = nullptr;
    bool shouldExit = false;
    bool backToMenu = false;
public:
    menu_screen();
    ~menu_screen();

    ftxui::Component makeUI();
    void loop();
    void setBackToMenu() { backToMenu = true; }
};

#endif //INC_4GEWINNT_CLIENT_MENU_SCREEN_H
