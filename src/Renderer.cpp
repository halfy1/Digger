#include "Renderer.hpp"

Renderer::Renderer(SDL_Renderer* sdl_renderer, int t_width, int t_height)
        : renderer(sdl_renderer), tile_width(t_width), tile_height(t_height) {}

void Renderer::render_board(Board& board) {
    for (size_t x = 0; x < BOARD_HEIGHT; ++x) {
        for (size_t y = 0; y < BOARD_WIDTH; ++y) {
            Tile& tile = board.get_tile(x, y);
            SDL_Rect rect = {
                tile.position.x * tile_width,
                tile.position.y * tile_height,
                tile_width,
                tile_height
            };

            SDL_SetRenderDrawColor(renderer, tile.color.r, tile.color.g, tile.color.b, tile.color.a);
            SDL_RenderFillRect(renderer, &rect);

            //grid lines
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void Renderer::render_collectibles(const Vector<Collectible*>& collectibles) {
    for (size_t i = 0; i < collectibles.size(); i++) {
        collectibles[i]->render(renderer, tile_width, tile_height);
    }
}

void Renderer::render_enemies(const Vector<Enemy>& enemies) {
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i].render(renderer, tile_width, tile_height);
    }
}

void Renderer::render_player(const Player& player) {
    player.render(renderer, tile_width, tile_height);
}


void Renderer::render_fireballs(const Vector<Fireball>& fireballs) {
    for (size_t i = 0; i < fireballs.size(); ++i) {
        fireballs[i].render(renderer, tile_width, tile_height);
    }
}