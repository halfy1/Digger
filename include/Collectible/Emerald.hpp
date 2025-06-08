#ifndef EMERALD_HPP
#define EMERALD_HPP

#include "Collectible.hpp"

class Emerald : public Collectible {
    public:
        Emerald(int x, int y, SDL_Color color);
        void update(Board& board) override;
        void render(SDL_Renderer* renderer, int tile_width, int tile_height) const override;
};

#endif