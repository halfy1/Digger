#include "Tile.hpp"

Tile::Tile(int x, int y, SDL_Color color) : position({x, y}), color(color) {}

void Tile::set_dug() {
    is_dug = true;
    color = {0, 0, 0, 255};  // Change color to black when dug
}