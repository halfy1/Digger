#ifndef BOARD_HPP
#define BOARD_HPP
#include "Tile.hpp"

const int BOARD_WIDTH = 15; /// The width of the game board.
const int BOARD_HEIGHT = 15; /// The height of the game board.

/**
 * @brief Represents the game board consisting of tiles.
 * 
 * The board manages the grid of tiles and provides access to individual tiles.
 * It also stores the dimensions of the board.
 */
class Board {
    public:
        Board(size_t width, size_t height); 
        Tile& get_tile(int x, int y);
        const int get_width() const { return BOARD_WIDTH; }
        const int get_height() const { return BOARD_HEIGHT; }

        bool is_inside(int x, int y) const {
        return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT;
        }
    public:
        const int tile_width, tile_height; /**< The width and height of a single tile in pixels. */
        Tile board[BOARD_WIDTH][BOARD_HEIGHT]; /**< A 2D array representing the tiles on the board. */
};

#endif