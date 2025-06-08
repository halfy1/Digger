#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include "Board.hpp"
#include "GameObject.hpp"
using namespace std;

class Collectible : public GameObject {
    public:
        Collectible(int x, int y, SDL_Color color);

        SDL_Point get_position() const;
        SDL_Color get_color() const;

        void set_position(int x, int y);

        virtual ~Collectible() = default;

    protected:
        SDL_Point position;
        SDL_Color color;
};

#endif
