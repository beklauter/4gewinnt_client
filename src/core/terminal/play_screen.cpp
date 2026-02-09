//
// Created by beklauter on 04.02.2026.
//

#include "play_screen.h"

play_screen::play_screen() {
    screen = new ftxui::ScreenInteractive(ScreenInteractive::Fullscreen());
}

play_screen::~play_screen() {
    if (screen) {
        delete screen;
        screen = nullptr;
    }
    if (gameHost) {
        gameHost->stopServer();
        delete gameHost;
    }
    if (gameClient) {
        gameClient->disconnect();
        delete gameClient;
    }
}

void play_screen::loop() {
    selectedMode = 0;
    inLobby = false;

    while (true) {
        if (!screen) {
            screen = new ftxui::ScreenInteractive(ScreenInteractive::Fullscreen());
        }

        if (selectedMode == 0) {
            auto title = Renderer([] {
                return text("SPIELMODUS WÄHLEN") | bold | center;
            });

            auto buttons = Container::Vertical({
                Button("🌐 SPIEL HOSTEN", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    selectedMode = 1;

                    if (gameHost) {
                        gameHost->stopServer();
                        delete gameHost;
                        gameHost = nullptr;
                    }

                    gameHost = new host();
                    if (gameHost->startServer("Mein Spiel")) {
                        inLobby = true;
                        screen->Exit();
                    } else {
                        selectedMode = 0;
                        delete gameHost;
                        gameHost = nullptr;
                    }
                }),
                Button("🔌 SPIEL BEITRETEN", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    selectedMode = 2;
                    screen->Exit();
                }),
                Button("🤖 EINZELSPIELER", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    selectedMode = 3;
                    screen->Exit();
                }),
                Button("◀ ZURÜCK", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    selectedMode = -1;
                    screen->Exit();
                })
            });

            auto ui = Container::Vertical({
                title,
                buttons
            }) | border | flex;

            ui = ui | CatchEvent([this](Event event) {
                if (event == Event::Escape) {
                    sounds::playsound("../assets/", "shutdown.wav");
                    screen->Exit();
                    selectedMode = -1;
                    return true;
                }
                return false;
            });

            screen->Loop(ui);

            if (selectedMode == -1) break;

            if (screen) {
                delete screen;
                screen = nullptr;
            }

        } else if (selectedMode == 1) {
            auto lobbyInfo = Renderer([this] {
                return vbox({
                    text("🌐 LOBBY GEÖFFNET") | bold | color(Color::Green) | center,
                    separator(),
                    hbox({
                        text("IP-Adresse: ") | bold,
                        text(gameHost ? gameHost->getLocalIP() : "N/A") | color(Color::Cyan)
                    }),
                    hbox({
                        text("Port: ") | bold,
                        text("54000") | color(Color::Cyan)
                    }),
                    separator(),
                    hbox({
                        text("Spieler: ") | bold,
                        text(gameHost ? std::to_string(gameHost->getClientCount() + 1) : "1") | color(Color::Yellow),
                        text(" / 2")
                    }),
                    separator(),
                    (gameHost && gameHost->getClientCount() >= 1) ?
                        text("Spieler verbunden! Starten Sie das Spiel.") | color(Color::Green) | center :
                        text("Warte auf Spieler...") | dim | center
                }) | border;
            });

            auto buttons = Container::Vertical({
                Button("▶ SPIEL STARTEN", [this] {
                    if (gameHost && gameHost->getClientCount() >= 1) {
                        sounds::playsound("../assets/", "button_click.wav");
                        NetworkMessage startMsg;
                        startMsg.type = MessageType::GAME_START;
                        gameHost->broadcastMessage(startMsg);

                        selectedMode = 3;
                        screen->Exit();
                    }
                }),
                Button("❌ ABBRECHEN", [this] {
                    // sounds::playsound("../assets/", "button_click.wav");
                    if (gameHost) {
                        gameHost->stopServer();
                        delete gameHost;
                        gameHost = nullptr;
                    }
                    selectedMode = 0;
                    inLobby = false;
                    screen->Exit();
                })
            });

            auto ui = Container::Vertical({
                lobbyInfo,
                buttons
            }) | border | flex;

            ui = ui | CatchEvent([this](Event event) {
                if (event == Event::Escape) {
                    sounds::playsound("../assets/", "shutdown.wav");
                    if (gameHost) {
                        gameHost->stopServer();
                        delete gameHost;
                        gameHost = nullptr;
                    }
                    selectedMode = 0;
                    inLobby = false;
                    screen->Exit();
                    return true;
                }
                return false;
            });

            screen->Loop(ui);

            if (screen) {
                delete screen;
                screen = nullptr;
            }

        } else if (selectedMode == 2) {
            auto ipInput = Input(&joinIP, "IP-Adresse eingeben");

            auto joinInfo = Renderer([] {
                return vbox({
                    text("🔌 SPIEL BEITRETEN") | bold | color(Color::Cyan) | center,
                    separator(),
                    text("Gib die IP-Adresse des Hosts ein:") | center
                }) | border;
            });

            auto buttons = Container::Vertical({
                ipInput,
                Button("✓ VERBINDEN", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    gameClient = new client();
                    if (gameClient->connectToServer(joinIP)) {
                        selectedMode = 4;
                        screen->Exit();
                    } else {
                        delete gameClient;
                        gameClient = nullptr;
                    }
                }),
                Button("◀ ZURÜCK", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    selectedMode = 0;
                    screen->Exit();
                })
            });

            auto ui = Container::Vertical({
                joinInfo,
                buttons
            }) | border | flex;

            ui = ui | CatchEvent([this](Event event) {
                if (event == Event::Escape) {
                    sounds::playsound("../assets/", "shutdown.wav");
                    selectedMode = 0;
                    screen->Exit();
                    return true;
                }
                return false;
            });

            screen->Loop(ui);

            if (screen) {
                delete screen;
                screen = nullptr;
            }

        } else if (selectedMode == 4) {
            auto lobbyInfo = Renderer([this] {
                return vbox({
                    text("⏳ WARTE AUF HOST") | bold | color(Color::Yellow) | center,
                    separator(),
                    hbox({
                        text("Verbunden mit: ") | bold,
                        text(joinIP) | color(Color::Cyan)
                    }),
                    separator(),
                    text("Host startet das Spiel...") | dim | center
                }) | border;
            });

            auto buttons = Container::Vertical({
                Button("❌ TRENNEN", [this] {
                    sounds::playsound("../assets/", "button_click.wav");
                    if (gameClient) {
                        gameClient->disconnect();
                        delete gameClient;
                        gameClient = nullptr;
                    }
                    selectedMode = 0;
                    screen->Exit();
                })
            });

            auto ui = Container::Vertical({
                lobbyInfo,
                buttons
            }) | border | flex;

            ui = ui | CatchEvent([this](Event event) {
                if (event == Event::Escape) {
                    sounds::playsound("../assets/", "shutdown.wav");
                    if (gameClient) {
                        gameClient->disconnect();
                        delete gameClient;
                        gameClient = nullptr;
                    }
                    selectedMode = 0;
                    screen->Exit();
                    return true;
                }
                return false;
            });

            if (gameClient) {
                auto originalCallback = gameClient->onMessageReceived;
                gameClient->onMessageReceived = [this, originalCallback](const NetworkMessage& msg) {
                    if (msg.type == MessageType::GAME_START) {
                        selectedMode = 3;
                        screen->Exit();
                    }
                    if (originalCallback) {
                        originalCallback(msg);
                    }
                };
            }

            screen->Loop(ui);

            if (screen) {
                delete screen;
                screen = nullptr;
            }

        } else if (selectedMode == 3) {
            game_screen game(gameHost, gameClient);
            game.loop();
            selectedMode = 0;
        }
    }
}

void play_screen::startGame() {
    if (screen) {
        screen->Exit();
    }

    game_screen game(gameHost, gameClient);
    game.loop();

    selectedMode = 0;
    inLobby = false;
}

