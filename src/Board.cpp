#include "Board.hpp"
#include <stdexcept>

Board::Board(size_t width, size_t height) : tile_width(width), tile_height(height) {
    for (size_t x = 0; x < BOARD_WIDTH; ++x) {
        for (size_t y = 0; y < BOARD_HEIGHT; ++y) {
            board[x][y] = Tile(x, y, {0, 0, 0, 0}); //Default color
        }
    }
}

Tile& Board::get_tile(int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        throw std::out_of_range("get_tile: coordinates out of bounds");
    }
    return board[x][y];
}
