#ifndef TILE_HPP
#define TILE_HPP

#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

struct Tile {
    Tile()=default;
    Tile(int x, int y, SDL_Color color);
    Tile& operator=(const Tile& rhs) = default;

    void set_dug();
    SDL_Point position;
    SDL_Color color;
    bool is_dug = false;
    bool has_entity = false; 
    bool has_collectible = false; 
};

#endif