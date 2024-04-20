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
    this->neighbors = std::move(neighbors);
}

// Parameterized constructor with position and neighbors
Tile::Tile(std::pair<int, int> coords, std::vector<Tile*>& neighbors) {
    this->coords = std::move(coords);
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
    this->isDebug = false;
    this->neighbors = std::move(neighbors);
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
// TODO should be const
void Tile::render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures, const bool isPaused) {
    enum textureIndices {
        flag, mine, num1, num2, num3, num4, num5, num6, num7, num8, hidden, revealed
    };

    // Only render the revealed texture if the game is paused
    if (isPaused) {
        renderSprite(window, textures[revealed]);
        return;
    }

    // Tile background
    if (isVisible) {
        renderSprite(window,textures[revealed]);
        if (hasFlag) { renderSprite(window, textures[flag]); }
        if (hasMine) { renderSprite(window,textures[mine]); }
    } else {
        renderSprite(window, textures[hidden]);
        if (hasFlag) { renderSprite(window, textures[flag]); }
    }
    // Render mines on top if debug mode is on
    if (isDebugMode() && isMine()) {
        renderSprite(window, textures[mine]);
    }
}

void Tile::renderSprite(sf::RenderWindow& window, const sf::Texture& texture) const {
    sf::Sprite sprite(texture);
    // Tiles are 32x32 pixels
    sprite.setPosition(static_cast<float>(coords.first) * 32, static_cast<float>(coords.second) * 32);
    window.draw(sprite);
}
