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
        src/game.cpp
        src/game.h
        src/host.cpp
        src/host.h
        src/client.cpp
        src/client.h
        src/console.cpp
        src/console.h
)

target_link_libraries(4gewinnt_client
        PRIVATE
        ws2_32
        ftxui::component
        ftxui::dom
        ftxui::screen
)
