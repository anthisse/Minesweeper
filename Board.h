#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include "Tile.h"
#include <random> // random numbers for the mines
#include <chrono> // random number seed

enum pos {
    TOP_LEFT, TOP_MID, TOP_RIGHT, MID_LEFT, MID_RIGHT, BOT_LEFT, BOT_MID, BOT_RIGHT
};

class Board {
private:
    std::pair<int, int> dimensions;
    std::vector<std::vector<Tile>> board;
    int mineCount;
    bool isDebug;
    bool isPaused;
    bool gameWon;
    bool gameOver;

    void initializeBoard();

    void populateBoard();

    Tile* findTileClicked(const sf::RenderWindow& window, const sf::Vector2i& mousePosition);


public:
    explicit Board(std::pair<int, int> dimensions, int mineCount);

    int getFlags() const;

    int getMines() const;

    int getRevealed() const;

    bool isDebugMode() const;

    bool paused() const;

    bool isGameOver() const;

    bool isGameWon() const;

    void setDebug(bool debug);

    void setPaused(bool p);

    void reset();

    void click(sf::RenderWindow& window, const sf::Vector2i& mousePosition, const bool& isLmb);

    void recursiveReveal(Tile& tile);

    void showMines();

    void render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures);

    void moveMine(Tile& clickedTile);
};

#endif //MINESWEEPER_BOARD_H
