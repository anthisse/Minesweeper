#ifndef MINESWEEPER_TRAY_GUI_H
#define MINESWEEPER_TRAY_GUI_H

#include <iostream>
#include <chrono>
#include <utility>
#include <SFML/Graphics.hpp>

class TrayGui {
private:
    std::pair<int,int> boardDimensions;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool paused;
    int mines;
    int flags;

public:
    explicit TrayGui(std::pair<int,int>& boardDimensions, const int &mines);

    long long getElapsedSeconds();

    bool isPaused() const;

    void setPaused(bool p);

    void incrementFlags();

    void decrementFlags();

    void render(sf::RenderWindow& window, std::vector<sf::Texture>& textures) const;

    void renderTimer(sf::RenderWindow& window, const sf::Texture& textures) const;

    void renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture) const;
};


#endif //MINESWEEPER_TRAY_GUI_H
