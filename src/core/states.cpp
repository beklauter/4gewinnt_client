//
// Created by beklauter on 04.02.2026.
//

#include "states.h"

states::GameState states::currentState = GameState::initialize;

void states::setState(GameState state) {
    currentState = state;
    std::cout << "states::setState -> new state set" << std::endl;
}

states::GameState states::getState() {
    return currentState;
}

std::ostream& operator<<(std::ostream& os, states::GameState state) {
    switch (state) {
        case states::GameState::initialize:   os << "initialize";   break;
        case states::GameState::loading:   os << "loading";   break;
        case states::GameState::menu:      os << "menu";      break;
        case states::GameState::shop:      os << "shop";      break;
        case states::GameState::lobby:     os << "lobby";     break;
        case states::GameState::game:      os << "game";      break;
        case states::GameState::gameover:  os << "gameover";  break;
        default:                           os << "unknown";   break;
    }
    return os;
}