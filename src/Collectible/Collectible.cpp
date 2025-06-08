#include "Collectible.hpp"

Collectible::Collectible(int x, int y, SDL_Color color) :
    position({x, y}), color(color) {}

SDL_Point Collectible::get_position() const {
    return position;
}

SDL_Color Collectible::get_color() const {
    return color;
}

void Collectible::set_position(int x, int y) {
    position = {x, y};
}