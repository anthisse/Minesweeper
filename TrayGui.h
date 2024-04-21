#ifndef MINESWEEPER_TRAY_GUI_H
#define MINESWEEPER_TRAY_GUI_H

#include <chrono>
#include <fstream>
#include <iomanip>
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
    std::chrono::duration<double, std::milli> totalPausedTime;
    bool paused;
    bool gameOver;
    bool gameWon;
    bool leaderboardDisplayed;
    bool hasSpace;
    bool lbCurrentlyOpen;
    std::string name;
    std::vector<sf::Sprite> buttonSprites;

public:
    explicit TrayGui(std::pair<int, int>& boardDimensions, const std::string& n);

    std::chrono::duration<double, std::milli> updateGameTime();

    void setGameOver(bool g);

    void setGameWon(bool w);

    void render(sf::RenderWindow& window, std::vector<sf::Texture>& textures,
                const int& numMines, const int& numFlags);

    void renderTimer(sf::RenderWindow& window, const sf::Texture& textures);

    void renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture,
                              const int& mines, const int& flags) const;

    bool click(sf::RenderWindow& window, const sf::Vector2i& mousePosition,
               const std::vector<sf::Texture>& tileTextures, Board& board);

    void displayLeaderboard();

    void writeScore(std::pair<std::string, std::string>& newEntry) const;

    static sf::Text initializeLeaderboardHeaderText(const sf::RenderWindow& window, const sf::Font& font);

    sf::Text initializeLeaderboardContentText(const sf::RenderWindow& window, const sf::Font& font);

    std::string getLeaderboardString(const std::string& lbName, const std::string& lbTime) const;

    std::string getLeaderboardString() const;

    static bool hasSpaces();
};

#endif //MINESWEEPER_TRAY_GUI_H
