//
// Created by beklauter on 04.02.2026.
//


#include "loadingscreen.h"

loadingscreen::loadingscreen() : screen(ScreenInteractive::Fullscreen()) {}

Component loadingscreen::makeUI() {
    return Renderer([&] {
        progress += 0.025f;
        if (progress >= 1.0f) {
            if (!done) {
                done = true;
                Sleep(1000);
                screen.Exit();
            }
            return vbox({ text("Loading Fertig!") | center });
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

void loadingscreen::loop() {
    screen.Loop(makeUI());
}
