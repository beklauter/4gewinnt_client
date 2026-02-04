//
// Created by beklauter on 04.02.2026.
//

#include "loader.h"

void loader::loadMenu() {
    std::cout << "Got into loader::loadMenu" << std::endl;
    states::GameState poststate = states::getState();

    states::setState(states::GameState::loading);

    states::GameState state = states::getState();

    std::cout << "Poststate was " << poststate << std::endl;
    std::cout << "Set the state to " << state << std::endl;

    bool terminal = console::setupConsole();

    if (terminal) {
        std::cout << "Console Setup Complete. Current State: " << states::getState() << std::endl;
        Sleep(1000);
        system("cls");
        // print::printWelcomeMessage();
        // sounds::playsound("../assets/", "happy.wav");
        // std::cin.ignore();

        loadingscreen loader;
        loader.loop();

        menu_screen menu;
        menu.loop();
    } else {
        std::cout << "Console Setup Failed." << std::endl;
    }
}
