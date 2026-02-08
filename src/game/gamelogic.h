//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_GAMELOGIC_H
#define INC_4GEWINNT_CLIENT_GAMELOGIC_H

#pragma once
#include "board.h"

class gamelogic {
private:
    board gameBoard;
    board::Cell currentPlayer;
    bool gameOver;
    board::Cell winner;

public:
    gamelogic();

    void reset();
    bool makeMove(int column);
    bool checkWin(int lastRow, int lastCol);
    int checkDirection(int row, int col, int dRow, int dCol, board::Cell player);
    bool isGameOver() const { return gameOver; }
    board::Cell getWinner() const { return winner; }
    board::Cell getCurrentPlayer() const { return currentPlayer; }
    void switchPlayer();
    board& getBoard() { return gameBoard; }
    const board& getBoard() const { return gameBoard; }
};


#endif //INC_4GEWINNT_CLIENT_GAMELOGIC_H