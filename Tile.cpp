#include <iostream>
#include "Tile.h"

// Default constructor
Tile::Tile() {
    this->coords = {0, 0};
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
}

// Fully parameterized constructor
Tile::Tile(std::pair<int, int> coords, bool hasMine, bool isRevealed, bool isFlagged, std::vector<Tile *>& neighbors) {
    this->coords = std::move(coords);
    this->hasMine = hasMine;
    this->isVisible = isRevealed;
    this->hasFlag = isFlagged;
    this->neighbors = std::move(neighbors);
}

// Parameterized constructor with position and neighbors
Tile::Tile(std::pair<int, int> coords, std::vector<Tile *>& neighbors) {
    this->coords = std::move(coords);
    this->hasMine = false;
    this->isVisible = false;
    this->hasFlag = false;
    this->neighbors = std::move(neighbors);
}

Tile::Tile(std::pair<int, int> coords) {
    this->coords = std::move(coords);
    this->hasMine = false;
    this->isVisible = false;
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

std::vector<Tile *> Tile::getNeighbors() const {
    std::cout << "col: " << coords.first << " row: " << coords.second << "\n";
    return this->neighbors;
}

std::pair<int, int> Tile::getCoords() const {
    return this->coords;
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