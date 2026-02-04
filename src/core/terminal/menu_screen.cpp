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
        Button("▶ SPIELEN", [&] { /* play */ }),
        Button("🛒 SHOP", [&] { /* shop */ }),
        Button("⚙ EINSTELLUNGEN", [&] { /* settings */ }),
        Button("❌ SCHLIESSEN", screen.ExitLoopClosure())
    });

    return Container::Vertical({
        title_renderer,
        buttons,
        sep_renderer,
        esc_renderer
    }) | center | flex;
}

void menu_screen::loop() {
    screen.Loop(makeUI());
}