#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>

using namespace ftxui;

int main() {
    std::array<std::array<int, 7>, 6> board{};
    board[5][0] = 1; board[5][1] = 2; board[4][3] = 1;  // Test

    auto canvas_board = [&]() -> Element {
        Canvas c(56, 30);

        for (int row = 0; row < 6; ++row) {
            for (int col = 0; col < 7; ++col) {
                int x = col * 8;
                int y = row * 5;

                if (board[row][col] == 0) {
                    c.DrawPointCircleFilled(x + 4, y + 2, 2, Color::GrayDark);
                } else if (board[row][col] == 1) {
                    c.DrawPointCircleFilled(x + 4, y + 2, 2, Color::Red);
                } else {
                    c.DrawPointCircleFilled(x + 4, y + 2, 2, Color::Yellow);
                }
            }
        }
        return canvas(&c) | xflex;
    };

    auto renderer = Renderer([=](bool) {
        return vbox({
            text("4 Gewinnt!") | bold | center,
            separator(),
            hbox({
                text("🔴 P1 | 🟡 P2") | center,
                canvas_board() | border | xflex,
            }) | center,
            separator(),
        });
    });

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer);

    return 0;
}
