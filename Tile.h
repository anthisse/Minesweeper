#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H

#include <vector> // For neighbors
#include <utility> // Includes std::pair
#include <SFML/Graphics.hpp>

class Tile {
private:
    std::pair<int, int> coords;
    bool hasMine;
    bool isVisible;
    bool hasFlag;
    bool isDebug;
    sf::Sprite sprite;
    std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    void drawSprite(sf::RenderWindow& window, const sf::Texture& texture);

public:
    // Default constructor
    Tile();

    // Fully parameterized constructor
    Tile(std::pair<int, int> coords, bool hasMine, bool isRevealed, bool isFlagged, std::vector<Tile*>& neighbors);

    // Parameterized constructor (just coords)
    Tile(std::pair<int, int> coords, std::vector<Tile*>& neighbors);

    explicit Tile(std::pair<int, int> coords);

    bool isMine() const;

    bool isRevealed() const;

    bool isFlagged() const;

    bool isDebugMode() const;

    sf::Sprite getSprite() const;

    std::pair<int, int> getCoords() const;

    std::vector<Tile*> getNeighbors() const;

    void setMine(bool mine);

    void setRevealed(bool visible);

    void setFlagged(bool flagged);

    void setDebug(bool debug);

    void setNeighbors(std::vector<Tile*>& n);

    void reset();

    void render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures, bool isPaused);

    int getNumMineNeighbors() const;

};

#endif //MINESWEEPER_TILE_H
