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
        src/menu/menu_screen.cpp
        src/menu/menu_screen.h
        src/lobby/lobbylogic.cpp
        src/lobby/lobbylogic.h
)

target_link_libraries(4gewinnt_client
        PRIVATE
        ws2_32
        ftxui::component
        ftxui::dom
        ftxui::screen
)
