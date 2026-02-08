//
// Created by beklauter on 04.02.2026.
//

#include "gamelogic.h"

gamelogic::gamelogic() : currentPlayer(board::Cell::PLAYER1), gameOver(false), winner(board::Cell::EMPTY) {
    reset();
}

void gamelogic::reset() {
    gameBoard.reset();
    currentPlayer = board::Cell::PLAYER1;
    gameOver = false;
    winner = board::Cell::EMPTY;
}

bool gamelogic::makeMove(int column) {
    if (gameOver) return false;

    int row = gameBoard.getLowestEmptyRow(column);
    if (row == -1) return false;

    if (gameBoard.dropPiece(column, currentPlayer)) {
        if (checkWin(row, column)) {
            gameOver = true;
            winner = currentPlayer;
        } else if (gameBoard.isFull()) {
            gameOver = true;
            winner = board::Cell::EMPTY; // Draw
        } else {
            switchPlayer();
        }
        return true;
    }

    return false;
}

void gamelogic::switchPlayer() {
    currentPlayer = (currentPlayer == board::Cell::PLAYER1) ? board::Cell::PLAYER2 : board::Cell::PLAYER1;
}

bool gamelogic::checkWin(int lastRow, int lastCol) {
    board::Cell player = gameBoard.getCell(lastRow, lastCol);
    if (player == board::Cell::EMPTY) return false;

    // horizontal
    if (checkDirection(lastRow, lastCol, 0, 1, player) + checkDirection(lastRow, lastCol, 0, -1, player) >= 3) {
        return true;
    }

    // vertical
    if (checkDirection(lastRow, lastCol, 1, 0, player) + checkDirection(lastRow, lastCol, -1, 0, player) >= 3) {
        return true;
    }

    // diagonal /
    if (checkDirection(lastRow, lastCol, 1, 1, player) + checkDirection(lastRow, lastCol, -1, -1, player) >= 3) {
        return true;
    }

    // reverse diagonal
    if (checkDirection(lastRow, lastCol, 1, -1, player) + checkDirection(lastRow, lastCol, -1, 1, player) >= 3) {
        return true;
    }

    return false;
}

int gamelogic::checkDirection(int row, int col, int dRow, int dCol, board::Cell player) {
    int count = 0;
    int r = row + dRow;
    int c = col + dCol;

    while (r >= 0 && r < board::ROWS && c >= 0 && c < board::COLS) {
        if (gameBoard.getCell(r, c) == player) {
            count++;
            r += dRow;
            c += dCol;
        } else {
            break;
        }
    }

    return count;
}

