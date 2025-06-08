#include "Fireball.hpp"
#include "Tile.hpp"

void Fireball::update(Board& board) {
    position.x += direction;

    if (!board.is_inside(position.x, position.y)) {
        destroyed = true;
    }
}

void Fireball::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { position.x * 53, position.y * 40, 53, 40 };
    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Fireball::render(SDL_Renderer* renderer, int tile_width, int tile_height) const {
    SDL_Rect rect = {
        position.x * tile_width,
        position.y * tile_height,
        tile_width,
        tile_height
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

Fireball::~Fireball() {
    // тело может быть пустым, если ничего не нужно освобождать
}