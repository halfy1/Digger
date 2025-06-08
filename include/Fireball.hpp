#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "GameObject.hpp"
#include "Board.hpp"
#include <SDL2/SDL.h>


class Fireball : public GameObject {
public:
    Fireball() = default;
    Fireball(SDL_Point position, int direction)
        : position(position), direction(direction),  active(true) {}
    virtual ~Fireball();
    void update(Board& board) override;
    void render(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, int tile_width, int tile_height) const;

    bool is_active() const { return active; }
    SDL_Point get_position() const { return position; }

    bool is_destroyed() const { return destroyed; }
    void destroy() {    
    destroyed = true;
    active = false;}

private:
    SDL_Color color = {255, 100, 0, 255};
    SDL_Point position;
    int direction; // -1 = left, +1 = right
    bool destroyed = false;
    bool active;
};

#endif
