//
// Created by beklauter on 04.02.2026.
//

#include "menu_screen.h"

menu_screen* g_menu_screen = nullptr;

menu_screen::menu_screen() : screen(ScreenInteractive::Fullscreen()) {
    g_menu_screen = this;
}

Component menu_screen::makeUI() {
    auto title_renderer = Renderer([ ] {
        return vbox({
            text("██╗  ██╗     ██████╗ ███████╗██╗    ██╗██╗███╗   ██╗███╗   ██╗████████╗") | color(Color::Red) | bold,
            text("██║  ██║    ██╔════╝ ██╔════╝██║    ██║██║████╗  ██║████╗  ██║╚══██╔══╝") | color(Color::Red),
            text("███████║    ██║  ███╗█████╗  ██║ █╗ ██║██║██╔██╗ ██║██╔██╗ ██║   ██║   ") | color(Color::Yellow) | bold,
            text("╚════██║    ██║   ██║██╔══╝  ██║███╗██║██║██║╚██╗██║██║╚██╗██║   ██║   ") | color(Color::Yellow),
            text("     ██║    ╚██████╔╝███████╗╚███╔███╔╝██║██║ ╚████║██║ ╚████║   ██║   ") | color(Color::Green) | bold,
            text("     ╚═╝     ╚═════╝ ╚══════╝ ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝   ╚═╝   ") | color(Color::Green),
        }) | center | flex_shrink;
    });

    Component sep_renderer = Renderer([] { return separatorDouble(); });

    Component esc_renderer = Renderer([] { return text("ESC zum Beenden") | dim; });

    Component buttons = Container::Vertical({
        Button("▶ SPIELEN", [&] { /* play func here */ }),
        Button("🛒 SHOP", [&] { /* shop func here */ }),
        Button("⚙ EINSTELLUNGEN", [&] { /* settings func here */ }),
        Button("❌ SCHLIESSEN", screen.ExitLoopClosure())
    });

    auto ui = Container::Vertical({
        title_renderer,
        buttons,
        sep_renderer,
        esc_renderer
    }) | center | flex;

    ui = ui | CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });
    return ui;
    /*
    return Container::Vertical({
        title_renderer,
        buttons,
        sep_renderer,
        esc_renderer
    }) | center | flex;

    */
}

void menu_screen::loop() {
    screen.Loop(makeUI());
}