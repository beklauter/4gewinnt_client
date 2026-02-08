cmake_minimum_required(VERSION 3.20)
project(4gewinnt_client)

set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

add_executable(4gewinnt_client
        src/main.cpp
        src/network/host.cpp
        src/network/host.h
        src/network/client.cpp
        src/network/client.h
        src/core/console.cpp
        src/core/console.h
        src/core/print.cpp
        src/core/print.h
        src/core/states.cpp
        src/core/states.h
        src/game/board.cpp
        src/game/board.h
        src/game/gamelogic.cpp
        src/game/gamelogic.h
        src/lobby/lobbylogic.cpp
        src/lobby/lobbylogic.h
        src/menu/shop.cpp
        src/menu/shop.h
        src/core/playerdata.cpp
        src/core/playerdata.h
        src/core/loader.cpp
        src/core/loader.h
        src/game/game.cpp
        src/game/game.h
        src/core/sounds.cpp
        src/core/sounds.h
        src/core/terminal/loading_screen.cpp
        src/core/terminal/loading_screen.h
        src/core/terminal/menu_screen.cpp
        src/core/terminal/menu_screen.h
        src/menu/menu.cpp
        src/menu/menu.h
        src/core/terminal/settings_screen.cpp
        src/core/terminal/settings_screen.h
        src/core/terminal/play_screen.cpp
        src/core/terminal/play_screen.h
        src/core/terminal/shop_screen.cpp
        src/core/terminal/shop_screen.h
        src/core/terminal/game_screen.cpp
        src/core/terminal/game_screen.h
)

target_link_libraries(4gewinnt_client
        PRIVATE
        ws2_32
        ftxui::component
        ftxui::dom
        ftxui::screen
)

target_compile_definitions(4gewinnt_client PRIVATE NOMINMAX)