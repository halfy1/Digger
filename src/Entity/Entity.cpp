#include "Entity.hpp"

Entity::Entity(int x, int y, int w, int h, SDL_Color c) :
    position({x, y}), velocity({0, 0}), width(w), height(h), color(c) {}

void Entity::move(Board& board) {
    int new_x = position.x + velocity.x; 
    int new_y = position.y + velocity.y;

    if (new_x >= 0 && new_x < board.get_width()) {
        position.x = new_x;
    }
    if (new_y >= 0 && new_y < board.get_height()) {
        position.y = new_y;
    }

    set_velocity(0, 0);
}


void Entity::render(SDL_Renderer* renderer, int tile_width, int tile_height) const {
    SDL_Rect rect = {
        position.x * tile_width,
        position.y * tile_height,
        width,
        height
    };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Entity::set_velocity(int dx, int dy) {
    velocity.x = dx;
    velocity.y = dy;
}

SDL_Point Entity::get_position() const { return position; }
SDL_Color Entity::get_color() const { return color; }

void Entity::set_position(int x, int y) {
    position.x = x;
    position.y = y;
}