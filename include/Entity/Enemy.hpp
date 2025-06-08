#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Vector/Vector.hpp"
#include "Queue/CircularQueue.hpp"
#include <chrono>

class Enemy : public Entity {
    public:
        Enemy() = default;
        Enemy(int x, int y, int w, int h, SDL_Color c);
        void update(Board& board) override {
            update(board, {/* player_position */});
        }
        void update(Board& board, SDL_Point player_position);
        void move(Board& board, SDL_Point player);

        void clear_path();
    private:
        Vector<SDL_Point> BFS(Board& board, SDL_Point position, SDL_Point target);
    private:
        Vector<SDL_Point> path;
        chrono::time_point< chrono::high_resolution_clock > last_update_time = chrono::high_resolution_clock::now();
        size_t current_path_index = 0;
};

#endif