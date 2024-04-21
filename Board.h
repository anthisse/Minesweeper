#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include "Tile.h"
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>
#include <chrono>

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

    std::vector<std::vector<Tile>> getBoard();

    std::pair<int,int> getDimensions() const;

    bool isDebugMode() const;

    bool paused() const;

    bool isGameOver() const;

    bool isGameWon() const;

    void setTileFlagged(std::pair<int, int> coords, bool flagged);

    void setTileMine(std::pair<int, int> coords, bool mine);

    void setTileRevealed(std::pair<int, int> coords, bool visible);

    void setDebug(bool debug);

    void setPaused(bool p);

    void setGameOver(bool over);

    void reset();

    // Figure out which tile was clicked
    void click(sf::RenderWindow& window, const sf::Vector2i& mousePosition, const bool& isLmb);

    void recursiveReveal(Tile& tile);

    Tile getTile(std::pair<int, int> coords);

    // TODO remove after debugging
    void print();

    void render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures);

};


#endif //MINESWEEPER_BOARD_H
