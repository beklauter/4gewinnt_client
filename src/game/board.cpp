//
// Created by beklauter on 04.02.2026.
//

#include "board.h"
#include <sstream>

board::board() {
    reset();
}

void board::reset() {
    grid.clear();
    grid.resize(ROWS, std::vector<Cell>(COLS, Cell::EMPTY));
}

bool board::dropPiece(int column, Cell player) {
    if (column < 0 || column >= COLS || isColumnFull(column)) {
        return false;
    }

    for (int row = ROWS - 1; row >= 0; --row) {
        if (grid[row][column] == Cell::EMPTY) {
            grid[row][column] = player;
            return true;
        }
    }

    return false;
}

board::Cell board::getCell(int row, int col) const {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return Cell::EMPTY;
    }
    return grid[row][col];
}

bool board::isColumnFull(int column) const {
    if (column < 0 || column >= COLS) return true;
    return grid[0][column] != Cell::EMPTY;
}

bool board::isFull() const {
    for (int col = 0; col < COLS; ++col) {
        if (!isColumnFull(col)) {
            return false;
        }
    }
    return true;
}

int board::getLowestEmptyRow(int column) const {
    if (column < 0 || column >= COLS) return -1;

    for (int row = ROWS - 1; row >= 0; --row) {
        if (grid[row][column] == Cell::EMPTY) {
            return row;
        }
    }
    return -1;
}

std::string board::toString() const {
    std::stringstream ss;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            switch (grid[row][col]) {
                case Cell::EMPTY:
                    ss << "· ";
                    break;
                case Cell::PLAYER1:
                    ss << "🔴";
                    break;
                case Cell::PLAYER2:
                    ss << "🟡";
                    break;
            }
        }
        ss << "\n";
    }
    return ss.str();
}

