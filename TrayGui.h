#ifndef MINESWEEPER_TRAY_GUI_H
#define MINESWEEPER_TRAY_GUI_H

#include <iostream>
#include <chrono>
#include <utility>
#include <SFML/Graphics.hpp>
#include "Board.h"

class TrayGui {
private:
    std::pair<int,int> boardDimensions;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool paused;
    bool isDebug;
    std::vector<sf::Sprite> buttonSprites;
    enum buttonSpriteIndices {
        face, pause, lb, debug
    };
    // TODO see TrayGui.cpp
    int mines;
    int flags;

public:
    TrayGui(std::pair<int,int>& boardDimensions, const int& mines);

    long long getElapsedSeconds();

    bool isPaused() const;

    bool isDebugOn() const;

    void setPaused(bool p);

    void setDebug(bool d);

    void incrementFlags();

    void decrementFlags();

    void render(sf::RenderWindow& window, std::vector<sf::Texture>& textures);

    void renderTimer(sf::RenderWindow& window, const sf::Texture& textures) const;

    void renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture) const;

    void click(const sf::RenderWindow& window, const sf::Vector2i& mousePosition, Board& board);


};


#endif //MINESWEEPER_TRAY_GUI_H
