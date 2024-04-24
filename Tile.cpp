#include <iostream>
#include "Tile.h"

// Default constructor
Tile::Tile() {
    this->coords = {0, 0};
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
    this->isDebug = false;
}

// Fully parameterized constructor
Tile::Tile(std::pair<int, int> coords, bool hasMine, bool isRevealed, bool isFlagged, std::vector<Tile*>& neighbors) {
    this->coords = std::move(coords);
    this->hasMine = hasMine;
    this->isVisible = isRevealed;
    this->hasFlag = isFlagged;
    this->isDebug = false;
    this->neighbors = neighbors;
}

// Parameterized constructor with position and neighbors
Tile::Tile(std::pair<int, int> coords, std::vector<Tile*>& neighbors) {
    this->coords = std::move(coords);
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
    this->isDebug = false;
    this->neighbors = neighbors;
}

Tile::Tile(std::pair<int, int> coords) {
    this->coords = std::move(coords);
    this->hasMine = false;
    this->isVisible = false;
    this->isDebug = false;
    this->hasFlag = false;
}

bool Tile::isMine() const {
    return this->hasMine;
}

bool Tile::isRevealed() const {
    return this->isVisible;
}

bool Tile::isFlagged() const {
    return this->hasFlag;
}

std::vector<Tile*> Tile::getNeighbors() const {
    return this->neighbors;
}

std::pair<int, int> Tile::getCoords() const {
    return this->coords;
}

bool Tile::isDebugMode() const {
    return this->isDebug;
}

sf::Sprite Tile::getSprite() const {
    return this->sprite;
}

void Tile::setMine(bool mine) {
    this->hasMine = mine;
}

void Tile::setRevealed(bool visible) {
    this->isVisible = visible;
}

void Tile::setFlagged(bool flagged) {
    this->hasFlag = flagged;
}

void Tile::setDebug(bool debug) {
    this->isDebug = debug;
}

void Tile::setNeighbors(std::vector<Tile*>& n) {
    this->neighbors = std::move(n);
}

void Tile::render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures, const bool isPaused,
                  const bool gameOver, const bool gameWon) {
    enum textureIndices {
        flag, num1, num2, num3, num4, num5, num6, num7, num8, mine, hidden, revealed
    };

    // Only render the revealed texture if the game is paused
    if (isPaused) {
        drawSprite(window, textures[revealed]);
        return;
    }

    // Tile background
    if (isVisible) {
        // Revealing tile logic
        if (hasMine) {
            if (gameWon) {
                drawSprite(window, textures[hidden]);
                drawSprite(window, textures[flag]);
                return;
            }
            if (hasFlag) {
                drawSprite(window, textures[hidden]);
                drawSprite(window, textures[flag]);
            }
            drawSprite(window, textures[hidden]);
            drawSprite(window, textures[mine]);
            return;
        }
        drawSprite(window, textures[revealed]);
        int numMineNeighbors = getNumMineNeighbors();
        if (numMineNeighbors != 0) { drawSprite(window, textures[numMineNeighbors]); }

    } else {
        // Hidden tile, not yet clicked
        drawSprite(window, textures[hidden]);
        if (hasFlag) { drawSprite(window, textures[flag]); }
    }
    // Render flags and mines on top if debug mode is on
    if (isDebugMode() && isMine()) {
        drawSprite(window, textures[mine]);
        if (isFlagged()) {
            drawSprite(window, textures[flag]);
        }
    }
}

int Tile::getNumMineNeighbors() const {
    int numMineNeighbors = 0;
    // Count number of neighbors that are mines
    for (const auto& neighbor: neighbors) {
        if (neighbor) {
            if (neighbor->isMine()) {
                numMineNeighbors++;
            }
        }
    }
    return numMineNeighbors;
}

// Reset the Tile to a blank Tile
void Tile::reset() {
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
}

void Tile::drawSprite(sf::RenderWindow& window, const sf::Texture& texture) {
    sprite.setTexture(texture);
    // Tiles are 32x32 pixels
    sprite.setPosition(static_cast<float>(coords.first) * 32, static_cast<float>(coords.second) * 32);
    window.draw(sprite);
}
