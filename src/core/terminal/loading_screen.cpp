
#include "loading_screen.h"

loading_screen::loading_screen() : screen(ScreenInteractive::Fullscreen()) {}

Component loading_screen::makeUI() {
    return Renderer([&] {
        progress += 0.025f;
        if (progress >= 1.0f) {
            if (!done) {
                done = true;
                Sleep(1000);
                screen.Exit();
            }
        }

        return vbox({
            hbox({
                text("4-Gewinnt") | border | bold,
                text("Loading...") | border | flex,
                text(std::to_string((int)(progress*100)) + "%") | border,
            }),

            gauge(progress) | color(Color::Green) | border | flex,

        }) | center | border | flex;
    });
}

void loading_screen::loop() {
    screen.Loop(makeUI());
}
