#include "Board.h"


Board::Board(std::pair<int, int> dimensions, int mineCount) {
    this->dimensions = std::move(dimensions);
    this->mineCount = mineCount;
    this->isDebug = false;
    this->gameOver = false;
    this->gameWon = false;
    this->isPaused = false;
    initializeBoard();
}

int Board::getFlags() const {
    int numFlagged = 0;
    for (const std::vector<Tile>& col: this->board) {
        for (const Tile& tile: col) {
            if (tile.isFlagged()) {
                numFlagged++;
            }
        }
    }
    return numFlagged;
}

int Board::getMines() const {
    return this->mineCount;
}

int Board::getRevealed() const {
    int numRevealed = 0;
    for (const std::vector<Tile>& column : this->board ) {
        for (const Tile& tile : column) {
            if (tile.isRevealed()) {
                numRevealed++;
            }
        }
    }
    return numRevealed;
}

bool Board::isDebugMode() const {
    return this->isDebug;
}

bool Board::paused() const {
    return this->isPaused;
}

bool Board::isGameOver() const {
    return this->gameOver;
}

bool Board::isGameWon() const {
    return this->gameWon;
}

void Board::populateBoard() {
    // Seed the random number generator and start generating
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> randomCol(0, dimensions.first - 1);
    std::uniform_int_distribution<> randomRow(0, dimensions.second - 1);
    int minesRemaining;
    // If the config asks for too many mines, just fill the board
    if (mineCount >= dimensions.first * dimensions.second) {
        minesRemaining = dimensions.first * dimensions.second;
    } else {
        minesRemaining = mineCount;
    }
    int colCoord;
    int rowCoord;
    while (minesRemaining != 0) {
        // Re-roll if there's already a mine
        colCoord = randomCol(rng);
        rowCoord = randomRow(rng);
        if (board[colCoord][rowCoord].isMine()) {
            continue;
        }
        board[colCoord][rowCoord].setMine(true);
        minesRemaining--;
    }
}

// Initialize an empty board
void Board::initializeBoard() {
    // Initialize the board with empty Tiles
    for (int col = 0; col < dimensions.first; col++) {
        std::vector<Tile> colVector;
        for (int row = 0; row < dimensions.second; row++) {
            std::pair<int, int> coords;
            std::vector<Tile*> neighbors = {};
            coords = {col, row};
            Tile tile = Tile(coords, neighbors);
            colVector.push_back(tile);
        }
        board.push_back(colVector);
        colVector.clear();
    }

    for (int i = 0; i < dimensions.first; i++) {
        for (int j = 0; j < dimensions.second; j++) {
            std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
            neighbors[TOP_LEFT] = (i > 0 && j > 0) ? &board[i - 1][j - 1] : nullptr;
            neighbors[TOP_MID] = (j > 0) ? &board[i][j - 1] : nullptr;
            neighbors[TOP_RIGHT] = (i < dimensions.first - 1 && j > 0) ? &board[i + 1][j - 1] : nullptr;
            // Middle colVector
            neighbors[MID_LEFT] = (i > 0) ? &board[i - 1][j] : nullptr;
            neighbors[MID_RIGHT] = (i < dimensions.first - 1) ? &board[i + 1][j] : nullptr;
            // Bottom colVector
            neighbors[BOT_LEFT] = (i > 0 && j < dimensions.second - 1) ? &board[i - 1][j + 1] : nullptr;
            neighbors[BOT_MID] = (j < dimensions.second - 1) ? &board[i][j + 1] : nullptr;
            neighbors[BOT_RIGHT] = (i < dimensions.first - 1 && j < dimensions.second - 1)
                                   ? &board[i + 1][j + 1] : nullptr;
            board[i][j].setNeighbors(neighbors);
        }
    }
    populateBoard();
}

// Figure out which tile was clicked
Tile* Board::findTileClicked(const sf::RenderWindow& window, const sf::Vector2i& mousePosition) {
    sf::Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
    for (std::vector<Tile>& col: board) {
        for (Tile& tile: col) {
            if (tile.getSprite().getGlobalBounds().contains(translatedPosition)) {
                return &tile;
            }
        }
    }
    // Click was not on any Tiles
    return nullptr;
}

void Board::setDebug(bool debug) {
    this->isDebug = debug;
    for (std::vector<Tile>& col: this->board) {
        for (Tile& tile: col) {
            tile.setDebug(debug);
        }
    }
}

void Board::setPaused(bool p) {
    this->isPaused = p;
}

// Reset the board
void Board::reset() {
    for (std::vector<Tile>& vec: board) {
        for (Tile& tile: vec) {
            tile.reset();
        }
    }
    populateBoard();
    this->gameOver = false;
    this->gameWon = false;
}

void Board::click(sf::RenderWindow& window, const sf::Vector2i& mousePosition, const bool& isLmb) {
    if (paused()) {
        return;
    }
    Tile* clickedTile = findTileClicked(window, mousePosition);
    // Split in two statements to ensure isRevealed() is never called on a nullptr (short-circuit eval not guaranteed)
    if (clickedTile == nullptr) {
        return;
    }
    // Move the mine if the first tile clicked is a mine
    if (getRevealed() == 0 && clickedTile->isMine()) {
        moveMine(*clickedTile);
    }
    if (clickedTile->isRevealed()) {
        return;
    }
    if (isLmb) {
        if (clickedTile->isFlagged()) {
            return;
        }
        std::vector<Tile*> neighbors = clickedTile->getNeighbors();
        recursiveReveal(*clickedTile);
        if (clickedTile->isMine()) {
            this->gameOver = true;
            this->gameWon = false;
            showMines();
            return;
        }
        int numRevealed = this->getRevealed();
        if (numRevealed == this->dimensions.first * this->dimensions.second - this->mineCount) {
            this->gameOver = true;
            this->gameWon = true;
            showMines();

            return;
        }
    } else {
        clickedTile->setFlagged(!clickedTile->isFlagged());
    }
}

void Board::moveMine(Tile& clickedTile) {
    // If the board is full or full -1 for some reason, do nothing (avoid infinite loop and a guaranteed win).
    if (mineCount - 1 >= dimensions.first * dimensions.second) {
        return;
    }
    clickedTile.setMine(false);
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> randomCol(0, dimensions.first - 1);
    std::uniform_int_distribution<> randomRow(0, dimensions.second - 1);
    int colCoord;
    int rowCoord;
    bool mineSet = false;
    while (!mineSet) {
        // Re-roll if there's already a mine
        colCoord = randomCol(rng);
        rowCoord = randomRow(rng);
        if (board[colCoord][rowCoord].isMine()) {
            continue;
        } else {
            board[colCoord][rowCoord].setMine(true);
            mineSet = true;
        }
    }
}

// Recursive, guaranteed to be fewer calls than the size of the board
void Board::recursiveReveal(Tile& tile) {
    if (tile.isMine() || tile.isFlagged()) {
        return;
    }
    tile.setRevealed(true);
    // Base case: Return early if there's a mine or flag next to me
    if (tile.getNumMineNeighbors() != 0 || tile.getNumFlagNeighbors()) {
        return;
    }

    std::vector<Tile*> neighbors = tile.getNeighbors();

    for (Tile* neighbor: neighbors) {
        if (neighbor == nullptr) {
            continue;
        }
        if (neighbor->isRevealed()) {
            continue;
        }
        neighbor->setRevealed(true);
        if (neighbor->getNumMineNeighbors() == 0) {
            recursiveReveal(*neighbor);

        }
    }
}

// Show all the mines
void Board::showMines() {
    for (std::vector<Tile>& column : this->board) {
        for (Tile& tile : column) {
            if (tile.isMine()) {
                tile.setRevealed(true);
            }
        }
    }
}

void Board::render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures) {
    for (auto& col: board) {
        for (auto& tile: col) {
            tile.render(window, textures, isPaused, gameOver, gameWon);
        }
    }
}
