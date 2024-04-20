#include "Board.h"


Board::Board(std::pair<int, int> dimensions, int mineCount) {
    this->dimensions = std::move(dimensions);
    this->mineCount = mineCount;
    this->isDebug = false;
    this->gameOver = false;
    this->isPaused = false;
    initializeBoard();
}

std::pair<int,int> Board::getDimensions() const {
    return this->dimensions;
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

void Board::populateBoard() {
    // Seed the random number generator and start generating
    std::random_device rd;
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> randomCol(0, dimensions.first - 1);
    std::uniform_int_distribution<> randomRow(0, dimensions.second - 1);
    int minesRemaining = mineCount;
    int colCoord;
    int rowCoord;
    while (minesRemaining != 0) {
        // Reroll if there's already a mine
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
    std::vector<Tile> colVector;
    std::pair<int, int> coords;
    std::vector<Tile*> neighbors = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    // Initialize the board with empty Tiles
    for (int col = 0; col < dimensions.first; col++) {
        for (int row = 0; row < dimensions.second; row++) {
            coords = {col, row};
            Tile tile = Tile(coords, neighbors);
            colVector.push_back(tile);
        }
        board.push_back(colVector);
        colVector.clear();
    }
    std::vector<std::vector<Tile>> newBoard;

    for (int col = 0; col < dimensions.first; col++) {
        for (int row = 0; row < dimensions.second; row++) {
            neighbors = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
            coords = {col, row};
            neighbors[TOP_LEFT] = (col > 0 && row > 0) ? &board[col - 1][row - 1] : nullptr;
            neighbors[TOP_MID] = (row > 0) ? &board[col][row - 1] : nullptr;
            neighbors[TOP_RIGHT] = (col < dimensions.first - 1 && row > 0) ? &board[col + 1][row - 1] : nullptr;
            // Middle colVector
            neighbors[MID_LEFT] = (col > 0) ? &board[col - 1][row] : nullptr;
            neighbors[MID_RIGHT] = (col < dimensions.first - 1) ? &board[col + 1][row] : nullptr;
            // Bottom colVector
            neighbors[BOT_LEFT] = (col > 0 && row < dimensions.second) ? &board[col - 1][row + 1] : nullptr;
            neighbors[BOT_MID] = (row < dimensions.second) ? &board[col][row + 1] : nullptr;
            neighbors[BOT_RIGHT] = (col < dimensions.first && row < dimensions.second) ? &board[col + 1][row + 1]
                                                                                       : nullptr;
            Tile tile = Tile(coords, neighbors);
            colVector.push_back(tile);
        }
        newBoard.push_back(colVector);
        colVector.clear();
    }
    board = std::move(newBoard);
    populateBoard();
}

std::vector<std::vector<Tile>> Board::getBoard() {
    return board;
}

void Board::setTileFlagged(std::pair<int, int> coords, bool flagged) {
    int column = coords.first;
    int row = coords.second;
    board[column][row].setFlagged(flagged);
}

void Board::setDebug(bool debug) {
    this->isDebug = debug;
    for (std::vector<Tile>& col : this->board) {
        for (Tile& tile : col) {
            tile.setDebug(debug);
        }
    }
}

void Board::setPaused(bool p) {
    this->isPaused = p;
}

void Board::setGameOver(bool over) {
    this->gameOver = over;
}

Tile Board::getTile(std::pair<int, int> coords) {
    int column = coords.first;
    int row = coords.second;
    std::cout << "accessing board at column " << coords.first << " and row " << coords.second << "\n";
    return board[column][row];
}

// Reset the board
void Board::reset() {
    for (auto& vec : board) {
        vec.clear();
    }
    board.shrink_to_fit();
    initializeBoard();
}

// Since columns are first then rows, this is anti-clockwise rotated 90 degrees!
void Board::print() {
    int i = 0;
    std::cout << "  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5\n";
    for (auto& col: board) {
        std::cout << i << " ";
        i++;
        for (int t = 0; t < dimensions.second; t++) {
            if (col[t].isFlagged()) {
                std::cout << "^ ";
            } else if (col[t].isMine()) {
                std::cout << "* ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

void Board::render(sf::RenderWindow& window, const std::vector<sf::Texture>& textures) {
    // TODO should be const!
    for ( auto& col : board) {
        for (auto& tile : col) {
            tile.render(window, textures, isPaused);
        }
    }
}
