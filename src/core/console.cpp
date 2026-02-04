//
// Created by beklauter on 02.02.2026.
//

#include "console.h"

bool console::setupConsole() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Console Preparation Complete" << std::endl;
    return true;
}
