#include "Window.h"

static std::string WindowTitle = "Brawls";

// some kind of Singleton
sf::RenderWindow& GetWindow() {
    static sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), WindowTitle);
    return *window;
}