#include "Enemy.hpp"

Enemy::Enemy(int x, int y, int w, int h, SDL_Color c)
    : Entity(x, y, w, h, c) {}

Vector<SDL_Point> Enemy::BFS(Board& board, SDL_Point start, SDL_Point target) {
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Right, Left, Down, Up
    CircularQueue<SDL_Point> queue;
    Vector<Vector<bool>> visited(board.get_width(), Vector<bool>(board.get_height(), false));
    Vector<Vector<SDL_Point>> parent(board.get_width(), Vector<SDL_Point>(board.get_height(), {-1, -1}));

    queue.enqueue(start);
    visited[start.x][start.y] = true;

    while (!queue.empty()) {
        SDL_Point current = queue.peek();
        queue.dequeue();

        if (current.x == target.x && current.y == target.y) {
            Vector<SDL_Point> path;
            SDL_Point step = target;

            while (!(step.x == start.x && step.y == start.y)) {
                path.push_back(step);
                step = parent[step.x][step.y];
            }
            path.push_back(start);

            Vector<SDL_Point> correct_order;
            for (int i = path.size() - 1; i >= 0; --i) {
                correct_order.push_back(path[i]);
            }
            return correct_order;
        }

        for (const auto& dir : directions) {
            SDL_Point neighbor = {current.x + dir[0], current.y + dir[1]};

            if (neighbor.x >= 0 && neighbor.x < board.get_width() &&
                neighbor.y >= 0 && neighbor.y < board.get_height() &&
                board.board[neighbor.x][neighbor.y].is_dug &&
                !visited[neighbor.x][neighbor.y]) {
                
                queue.enqueue(neighbor);
                visited[neighbor.x][neighbor.y] = true;
                parent[neighbor.x][neighbor.y] = current;
            }
        }
    }

    return {};
}

void Enemy::move(Board& board, SDL_Point player_position) {
    auto current_time = std::chrono::high_resolution_clock::now();
        chrono::duration<double> time_since_last_update(current_time - last_update_time);
        if (time_since_last_update.count() >= 0.5) { // Update every 0.5 seconds
            if (path.empty() || current_path_index >= path.size()) {
                path = BFS(board, position, player_position);
                current_path_index = 0;
            } else if (player_position.x != path.back().x && player_position.y != path.back().y) {
                path = BFS(board, position, player_position);
                current_path_index = 0;
            }

            if (!path.empty() && current_path_index < path.size()) {
                SDL_Point nextStep = path[current_path_index];
                set_position(nextStep.x, nextStep.y);
                current_path_index++; 
            }
            last_update_time = current_time;
        }
}

void Enemy::clear_path() {
    while (!path.empty()) {
        path.pop_back();
    }
}

void Enemy::update(Board& board, SDL_Point player_position) {
    move(board, player_position);
}