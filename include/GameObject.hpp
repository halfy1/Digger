#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <SDL2/SDL.h>

class Board;

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(Board& board) = 0;
    virtual void render(SDL_Renderer* renderer, int tile_width, int tile_height) const = 0;
};

#endif
