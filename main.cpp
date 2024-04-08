#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

// TODO files directory might need to be moved into cmake-build-debug

int main() {
    // Open the config file
    std::ifstream configFile = std::ifstream("files/config.cfg");
    if (!configFile.good()) {
        // Couldn't open file
        throw std::ifstream::failure("Failed to open file!");
    }

    // Render the welcome menu
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Magenta);

    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}