#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include "Board.hpp"
#include "GameObject.hpp"
using namespace std;


class Entity : public GameObject {
    public:
        Entity() = default;
        Entity(int x, int y, int w, int h, SDL_Color c);

        void set_position(int x, int y);    
        void set_velocity(int dx, int dy);

        SDL_Point get_velocity() const { return velocity; };
        SDL_Point get_position() const;
        SDL_Color get_color() const; 

        virtual void move(Board& board);
        virtual void render(SDL_Renderer* renderer, int tile_width, int tile_height) const;
        virtual ~Entity() = default;

    protected:
        SDL_Color color;
        SDL_Point position;
        SDL_Point velocity;
        int width, height;
};

#endif

