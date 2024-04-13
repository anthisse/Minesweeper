#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include "Tile.h"
#include <vector>
#include <utility> // includes std::pair

enum pos {
    TOP_LEFT, TOP_MID, TOP_RIGHT, MID_LEFT, MID_RIGHT, BOT_LEFT, BOT_MID, BOT_RIGHT
};

class Board {
private:
    std::pair<int, int> dimensions;
    std::vector<std::vector<Tile>> board;
    int mineCount;

    void initializeBoard();

    void populateBoard();


public:
    explicit Board(std::pair<int, int> dimensions, int mineCount);

    std::vector<std::vector<Tile>> getBoard();

    void setTileFlagged(std::pair<int, int> coords, bool flagged);

    void setTileMine(std::pair<int, int> coords, bool mine);

    void setTileRevealed(std::pair<int, int> coords, bool visible);

    Tile getTile(std::pair<int, int> coords);

    // TODO remove after debugging
    void printBoard();

};


#endif //MINESWEEPER_BOARD_H
