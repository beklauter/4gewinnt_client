//
// Created by beklauter on 04.02.2026.
//

#include "game_screen.h"
#include <sstream>

game_screen::game_screen(host* h, client* c)
    : screen(ScreenInteractive::Fullscreen()),
      gameHost(h),
      gameClient(c),
      isHost(h != nullptr),
      isMultiplayer(h != nullptr || c != nullptr),
      selectedColumn(3) {

    // Player 1 = Red, Player 2 = Yellow
    if (isHost) {
        myPlayerId = 0;
        myColor = board::Cell::PLAYER1;
    } else {
        myPlayerId = 1;
        myColor = board::Cell::PLAYER2;
    }

    if (gameHost) {
        gameHost->onMessageReceived = [&](int playerId, const NetworkMessage& msg) {
            if (msg.type == MessageType::MOVE) {
                game.makeMove(msg.column);
                waitingForRemote = false;
            }
        };
    }

    if (gameClient) {
        gameClient->onMessageReceived = [&](const NetworkMessage& msg) {
            if (msg.type == MessageType::MOVE) {
                game.makeMove(msg.column);
                waitingForRemote = false;
            }
        };
    }
}

game_screen::~game_screen() = default;

Component game_screen::makeGameBoard() {
    return Renderer([&] {
        std::vector<Element> rows;

        std::vector<Element> colHeaders;
        for (int col = 0; col < board::COLS; ++col) {
            std::string marker = (col == selectedColumn) ? "▼" : " ";
            colHeaders.push_back(text(marker + std::to_string(col + 1)) | center);
        }
        rows.push_back(hbox(colHeaders) | center);

        for (int row = 0; row < board::ROWS; ++row) {
            std::vector<Element> cells;
            for (int col = 0; col < board::COLS; ++col) {
                board::Cell cell = game.getBoard().getCell(row, col);
                std::string cellChar;
                Color cellColor = Color::White;

                if (cell == board::Cell::EMPTY) {
                    cellChar = "⚪";
                } else if (cell == board::Cell::PLAYER1) {
                    cellChar = "🔴";
                    cellColor = Color::Red;
                } else {
                    cellChar = "🟡";
                    cellColor = Color::Yellow;
                }

                cells.push_back(text(cellChar) | color(cellColor) | center);
            }
            rows.push_back(hbox(cells) | center);
        }

        return vbox(rows) | border | center;
    });
}

Component game_screen::makeGameInfo() {
    return Renderer([&] {
        std::string currentPlayerStr = (game.getCurrentPlayer() == board::Cell::PLAYER1) ? "Rot 🔴" : "Gelb 🟡";

        if (game.isGameOver()) {
            if (game.getWinner() == board::Cell::EMPTY) {
                return vbox({
                    text("UNENTSCHIEDEN!") | bold | color(Color::Yellow) | center,
                    text("Drücke ENTER für neues Spiel") | dim | center
                }) | border;
            } else {
                std::string winnerStr = (game.getWinner() == board::Cell::PLAYER1) ? "Rot 🔴" : "Gelb 🟡";
                return vbox({
                    text("GEWINNER: " + winnerStr) | bold | color(Color::Green) | center,
                    text("Drücke ENTER für neues Spiel") | dim | center
                }) | border;
            }
        }

        return vbox({
            hbox({
                text("Aktueller Spieler: ") | bold,
                text(currentPlayerStr)
            }) | center,
            text("← → zum Bewegen | ENTER zum Einwerfen | ESC zum Beenden") | dim | center
        }) | border;
    });
}

Component game_screen::makeChatBox() {
    return Renderer([&] {
        std::vector<Element> messages;

        int maxMessages = 5;
        int startIdx = (chatMessages.size() > maxMessages) ? chatMessages.size() - maxMessages : 0;

        for (int i = startIdx; i < chatMessages.size(); ++i) {
            messages.push_back(text(chatMessages[i]) | dim);
        }

        return vbox({
            text("CHAT") | bold | center,
            separator(),
            vbox(messages),
            separator(),
            text("Eingabe: " + chatInput)
        }) | border | size(HEIGHT, LESS_THAN, 10);
    });
}

void game_screen::handleNetworkMove(int column) {
    game.makeMove(column);
}

void game_screen::sendMove(int column) {
    NetworkMessage msg;
    msg.type = MessageType::MOVE;
    msg.playerId = myPlayerId;
    msg.column = column;
    msg.playerColor = (myColor == board::Cell::PLAYER1) ? 1 : 2;

    if (gameHost) {
        gameHost->broadcastMessage(msg);
    } else if (gameClient) {
        gameClient->sendMessage(msg);
    }
}

Component game_screen::makeUI() {
    auto gameBoard = Renderer([&] {
        std::vector<Element> rows;

        // Column numbers
        std::vector<Element> colHeaders;
        for (int col = 0; col < board::COLS; ++col) {
            std::string marker = (col == selectedColumn) ? "▼" : " ";
            colHeaders.push_back(text(marker + std::to_string(col + 1)) | center);
        }
        rows.push_back(hbox(colHeaders) | center);

        // Board grid
        for (int row = 0; row < board::ROWS; ++row) {
            std::vector<Element> cells;
            for (int col = 0; col < board::COLS; ++col) {
                board::Cell cell = game.getBoard().getCell(row, col);
                std::string cellChar;
                Color cellColor = Color::White;

                if (cell == board::Cell::EMPTY) {
                    cellChar = "⚪";
                } else if (cell == board::Cell::PLAYER1) {
                    cellChar = "🔴";
                    cellColor = Color::Red;
                } else {
                    cellChar = "🟡";
                    cellColor = Color::Yellow;
                }

                cells.push_back(text(cellChar) | color(cellColor) | center);
            }
            rows.push_back(hbox(cells) | center);
        }

        return vbox(rows) | border | center;
    });

    auto gameInfo = Renderer([&] {
        std::string myColorStr = (myColor == board::Cell::PLAYER1) ? "Rot 🔴" : "Gelb 🟡";
        std::string currentPlayerStr = (game.getCurrentPlayer() == board::Cell::PLAYER1) ? "Rot 🔴" : "Gelb 🟡";

        if (game.isGameOver()) {
            if (game.getWinner() == board::Cell::EMPTY) {
                return vbox({
                    text("UNENTSCHIEDEN!") | bold | color(Color::Yellow) | center,
                    text("Drücke ENTER für neues Spiel") | dim | center
                }) | border;
            } else {
                std::string winnerStr = (game.getWinner() == board::Cell::PLAYER1) ? "Rot 🔴" : "Gelb 🟡";
                return vbox({
                    text("GEWINNER: " + winnerStr) | bold | color(Color::Green) | center,
                    text("Drücke ENTER für neues Spiel") | dim | center
                }) | border;
            }
        }

        bool isMyTurn = (game.getCurrentPlayer() == myColor);

        return vbox({
            hbox({
                text("Du spielst als: ") | bold,
                text(myColorStr) | color(myColor == board::Cell::PLAYER1 ? Color::Red : Color::Yellow)
            }) | center,
            hbox({
                text("Aktueller Spieler: ") | bold,
                text(currentPlayerStr)
            }) | center,
            separator(),
            isMyTurn ? text("DEIN ZUG!") | bold | color(Color::Green) | center
                     : text("Warte auf Gegner...") | dim | color(Color::Yellow) | center,
            separator(),
            text(isMyTurn ? "← → zum Bewegen | ENTER zum Einwerfen | ESC zum Beenden"
                          : "Warte auf Gegner...") | dim | center
        }) | border;
    });

    auto ui = Container::Vertical({
        gameBoard,
        gameInfo
    });

    return ui | CatchEvent([&](Event event) {
        bool isMyTurn = (game.getCurrentPlayer() == myColor);

        if (event == Event::Escape) {
            sounds::playsound("../assets/", "shutdown.wav");
            screen.ExitLoopClosure()();
            return true;
        }

        if (game.isGameOver()) {
            if (event == Event::Return) {
                game.reset();
                selectedColumn = 3;
                waitingForRemote = false;
                return true;
            }
            return false;
        }

        if (!isMyTurn && isMultiplayer) {
            return false;
        }

        if (event == Event::ArrowLeft) {
            selectedColumn = (selectedColumn - 1 + board::COLS) % board::COLS;
            return true;
        }

        if (event == Event::ArrowRight) {
            selectedColumn = (selectedColumn + 1) % board::COLS;
            return true;
        }

        if (event == Event::Return) {
            if (game.makeMove(selectedColumn)) {
                sounds::playsound("../assets/", "button_click.wav");

                if (isMultiplayer) {
                    sendMove(selectedColumn);
                    waitingForRemote = true;
                }

                if (game.isGameOver()) {
                    sounds::playsound("../assets/", "game_over.wav");
                }
            }
            return true;
        }

        return false;
    }) | Renderer([&](Element inner) {
        return vbox({
            text("4-GEWINNT") | bold | color(Color::GreenYellow) | center,
            separator(),
            inner
        }) | center | flex;
    });
}

void game_screen::loop() {
    screen.Loop(makeUI());
}

