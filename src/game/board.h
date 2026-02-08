//
// Created by beklauter on 04.02.2026.
//

#ifndef INC_4GEWINNT_CLIENT_BOARD_H
#define INC_4GEWINNT_CLIENT_BOARD_H

#pragma once
#include <vector>
#include <string>

class board {
public:
    static const int ROWS = 6;
    static const int COLS = 7;

    enum class Cell {
        EMPTY,
        PLAYER1,
        PLAYER2
    };

private:
    std::vector<std::vector<Cell>> grid;

public:
    board();

    void reset();
    bool dropPiece(int column, Cell player);
    Cell getCell(int row, int col) const;
    bool isColumnFull(int column) const;
    bool isFull() const;
    int getLowestEmptyRow(int column) const;

    std::string toString() const;
};

#endif //INC_4GEWINNT_CLIENT_BOARD_H