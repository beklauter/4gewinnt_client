#include "menu_screen.h"
#include "play_screen.h"

menu_screen* g_menu_screen = nullptr;

menu_screen::menu_screen() {
    g_menu_screen = this;
    screen = new ftxui::ScreenInteractive(ScreenInteractive::Fullscreen());
}

menu_screen::~menu_screen() {
    if (screen) {
        delete screen;
        screen = nullptr;
    }
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
        Button("▶ SPIELEN", [&] {
            sounds::playsound("../assets/", "button_click.wav");
            screen->Exit();
        }),
        Button("🛒 SHOP", [&] {
            sounds::playsound("../assets/", "button_click.wav");
        }),
        Button("⚙ EINSTELLUNGEN", [&] {
            sounds::playsound("../assets/", "button_click.wav");
        }),
        Button("❌ SCHLIESSEN", [&] {
            sounds::playsound("../assets/", "shutdown.wav");
            shouldExit = true;
            screen->Exit();
        })
    });

    auto ui = Container::Vertical({
        title_renderer,
        buttons,
        sep_renderer,
        esc_renderer
    }) | center | flex;

    ui = ui | CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            sounds::playsound("../assets/", "shutdown.wav");
            shouldExit = true;
            screen->Exit();
            return true;
        }
        return false;
    });
    return ui;
}

void menu_screen::loop() {
    sounds::playsound("../assets/", "startup.wav");

    while (!shouldExit) {
        if (!screen) {
            screen = new ftxui::ScreenInteractive(ScreenInteractive::Fullscreen());
        }

        backToMenu = false;
        screen->Loop(makeUI());

        if (shouldExit) {
            break;
        }

        if (!backToMenu) {
            play_screen playScreen;
            playScreen.loop();
        }
    }
}
