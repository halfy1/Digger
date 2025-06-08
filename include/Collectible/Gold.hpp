#ifndef GOLD_HPP
#define GOLD_HPP

#include "Collectible.hpp"
#include <chrono>
#include "Vector/Vector.hpp"

class Gold : public Collectible {
    public:

        Gold(int x, int y, SDL_Color color);

        void update(Board& board) override;
        bool try_push(const SDL_Point& dir, Board& board, Vector<Collectible*>& collectibles);
        void render(SDL_Renderer* renderer, int tile_width, int tile_height) const override;
        bool get_is_broken() const { return is_broken; };
    private:
        int fall_distance = 0;
        chrono::time_point<chrono::high_resolution_clock> last_update_time = chrono::high_resolution_clock::now();
        bool is_falling = false;
        bool is_broken = false;
};

#endif