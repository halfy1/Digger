#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Board.hpp"
#include "Vector/Vector.hpp"
#include "Collectible.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Fireball.hpp"

class Renderer {
public:
    Renderer(SDL_Renderer* sdl_renderer, int t_width, int t_height);

    void render_board(Board& board);
    void render_collectibles(const Vector<Collectible*>& entities);
    void render_enemies(const Vector<Enemy>& enemies);
    void render_player(const Player& player);
    void render_fireballs(const Vector<Fireball>& fireballs);

private:
    SDL_Renderer* renderer; 
    int tile_width, tile_height;
};

#endif
