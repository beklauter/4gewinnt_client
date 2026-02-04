//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_MENU_SCREEN_H
#define INC_4GEWINNT_CLIENT_MENU_SCREEN_H


#pragma once
#include <ftxui/component/component.hpp>
#include "../states.h"
#include "ftxui/component/screen_interactive.hpp"
#include <conio.h>
#include <iostream>
#include "../sounds.h"

class menu_screen;

using namespace ftxui;

extern menu_screen* g_menu_screen;

class menu_screen {
    ftxui::ScreenInteractive screen;
public:
    menu_screen();

    ftxui::Component makeUI();
    void loop();
};



#endif //INC_4GEWINNT_CLIENT_MENU_SCREEN_H