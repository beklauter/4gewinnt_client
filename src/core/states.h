//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_STATES_H
#define INC_4GEWINNT_CLIENT_STATES_H

#include <iostream>

class states {
public:
    enum class GameState {
        initialize, // post startup variable -> changes into loading in loader.cpp
        loading,
        menu,
        shop,
        lobby,
        game,
        gameover
    };

    static void setState(GameState state);
    static GameState getState();

private:
    static GameState currentState;
};

std::ostream& operator<<(std::ostream& os, states::GameState state); // -> for printing the state


#endif //INC_4GEWINNT_CLIENT_STATES_H