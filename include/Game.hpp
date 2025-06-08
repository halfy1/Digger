#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Renderer.hpp"
#include "Collectible.hpp"
#include "Gold.hpp"
#include "Emerald.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Fireball.hpp"
#include <fstream>

enum class GameResult {
    BackToMenu,
    Quit
};

class Game {
    public:
        Game() = default;
        bool init(SDL_Renderer* renderer);
        GameResult run();

        void update();
        void handle_events();
        void render();
        void fire_fireball();

        SDL_Point get_player_position();
        SDL_Point get_player_spawn_position() { return player_spawn_position; };
    
        void respawn();
        void kill_enemy(size_t index);
        void remove_collectible(size_t index);
        ~Game();
    private:
        void deallocate();
    private:
        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 600;
        const int TILE_WIDTH = 53;
        const int TILE_HEIGHT = 40;
    private:
        bool back_to_menu = false;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Renderer* board_renderer = nullptr;
        Vector<Collectible*> collectibles;
        Vector<Enemy> enemies;
        Vector<SDL_Point> enemy_spawns;
        Vector<Fireball> fireballs;
        Board* board = nullptr; 
        Player player;
        SDL_Point player_spawn_position;
        std::chrono::time_point<std::chrono::high_resolution_clock> last_death_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> last_spawn_time = std::chrono::high_resolution_clock::now();
        const double respawn_time = 1000;
        int enemy_count;
        bool is_running = false;
};

#endif