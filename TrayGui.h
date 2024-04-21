#ifndef MINESWEEPER_TRAY_GUI_H
#define MINESWEEPER_TRAY_GUI_H

#include <iostream>
#include <chrono>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "file_read_exception.h"

class TrayGui {
private:
    std::pair<int, int> boardDimensions;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> pausedStartTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> pausedEndTime;
    std::chrono::duration<double, std::milli> totalPausedTime = {};
    bool paused;
    bool gameOver;
    bool gameWon;
    std::vector<sf::Sprite> buttonSprites;

public:
    explicit TrayGui(std::pair<int, int>& boardDimensions);

    std::chrono::duration<double, std::milli> updateGameTime();

    void setGameOver(bool g);

    void setGameWon(bool w);

    void render(sf::RenderWindow& window, std::vector<sf::Texture>& textures,
                const int& numMines, const int& numFlags);

    void renderTimer(sf::RenderWindow& window, const sf::Texture& textures);

    void renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture,
                              const int& mines, const int& flags) const;

    void click(sf::RenderWindow& window, const sf::Vector2i& mousePosition,
               const std::vector<sf::Texture>& tileTextures, Board& board);

    void openLeaderboard();

    static sf::Text initializeLeaderboardHeaderText(const sf::RenderWindow& window, const sf::Font& font);

    static sf::Text initializeLeaderboardContentText(const sf::RenderWindow& window, const sf::Font& font);

};

#endif //MINESWEEPER_TRAY_GUI_H
