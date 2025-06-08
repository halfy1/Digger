#include "Game.hpp"

bool Game::init(SDL_Renderer* renderer) {
    this->renderer = renderer;
    // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    //         SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
    //         return false;
    // }
    // window = SDL_CreateWindow("Digger Game",
    //                         SDL_WINDOWPOS_CENTERED,
    //                         SDL_WINDOWPOS_CENTERED,
    //                         SCREEN_WIDTH,
    //                         SCREEN_HEIGHT,
    //                         SDL_WINDOW_SHOWN);
    // if (!window) {
    //     SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
    //     return false;
    // }
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    // if (!renderer) {
    //     SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
    //     return false;
    // }

    board = new Board(TILE_WIDTH, TILE_HEIGHT);
    board_renderer = new Renderer(renderer, TILE_WIDTH, TILE_HEIGHT);
    //! loading level
    ifstream ifs("/workspaces/Digger/Level.txt");
    if (!ifs) {
        cerr << "Can't find load level." << endl;
        return false;
    }

    int x = 0, y = 0;
    char tile_value;
    
    while (ifs.get(tile_value)) {
        if (tile_value == '\n') {
            x = 0;
            ++y;
            continue;
        }
        Tile& tile = board->get_tile(x, y);
        switch (tile_value) {
            case '0':
                // Empty
                tile = Tile(x, y, {0, 0, 0, 0});
                tile.is_dug = true;
                break;  
            case '1': // Regular tile (block)
                tile = Tile(x, y, {104, 72, 35, 1});
                tile.is_dug = false;
                break;
            case '2': // Player tile
                player = Player(x, y, TILE_WIDTH, TILE_HEIGHT, {149, 20, 121, 1});
                tile.has_entity = true;
                player_spawn_position.x = x;
                player_spawn_position.y = y;
                break;
            case '3': // Enemy tile
                enemies.push_back(Enemy(x, y, TILE_WIDTH, TILE_HEIGHT, {255, 0, 0, 1}));
                enemy_spawns.push_back({x, y});
                tile.has_entity = true;
                break;
            case '4': // Emerald tile
                collectibles.push_back(new Emerald(x, y, {0, 255, 0, 255}));
                tile.has_collectible = true;
                break;
            case '5': // Gold tile
                collectibles.push_back(new Gold(x, y, {255, 215, 0, 255}));
                tile.has_collectible = true;
                break;
            default:
                tile = Tile(x, y, {0, 0, 0, 255});
                break;
        }

        ++x;
    }
    ifs.close();
    //!
    enemy_count = enemies.size();
    is_running = true;
    return true;
}

GameResult Game::run() {
    std::cout << "Game started!" << std::endl; // отладка
    is_running = true;
    
    while (is_running) {
        handle_events();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }

    return back_to_menu ? GameResult::BackToMenu : GameResult::Quit;
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    board_renderer->render_board(*board);
    board_renderer->render_collectibles(collectibles);
    board_renderer->render_enemies(enemies);
    board_renderer->render_player(player);
    board_renderer->render_fireballs(fireballs);
    SDL_RenderPresent(renderer);
}

void Game::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
            back_to_menu = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    is_running = false;
                    back_to_menu = true;
                    break;
                case SDLK_SPACE:
                    fire_fireball();
                    break;
            }
        }

        player.handle_input(e);
    }
}

void Game::fire_fireball() {
    int dir = player.get_facing_direction();
    SDL_Point pos = player.get_position();
    SDL_Color color = {255, 128, 0, 255}; // ярко-оранжевый

    // Смещение fireball на 1 тайл в направлении взгляда
    Fireball fb({pos.x + dir, pos.y}, dir);
    fireballs.push_back(fb);
}

void Game::respawn() {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_since_death(current_time - last_death_time);
    if (time_since_death.count() >= respawn_time) {
        player.set_position(player_spawn_position.x, player_spawn_position.y);
        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].set_position(enemy_spawns[i].x, enemy_spawns[i].y);
            enemies[i].clear_path();
        }
    }
}

void Game::kill_enemy(size_t index) {
    std::swap(enemies[index], enemies[enemies.size()-1]);
    enemies.pop_back();
}

void Game::remove_collectible(size_t index) {
    delete collectibles[index];
    std::swap(collectibles[index], collectibles[collectibles.size()-1]);
    collectibles.pop_back();
}

void Game::update() {
    if (player.get_lives() == 0 || collectibles.empty()) {
        is_running = false;
        back_to_menu = true;
        return;
    }

    // Восстановление убитых врагов
    if (enemies.size() < enemy_count) {
        int range = enemies.size() + 1;
        int num = rand() % range;
        enemies.push_back(Enemy(enemy_spawns[num].x, enemy_spawns[num].y, TILE_WIDTH, TILE_HEIGHT, {255, 0, 0, 255}));
    }

    // === Движение игрока + попытка толкнуть мешок ===
    SDL_Point move_dir = player.get_move_direction();
    if (move_dir.x != 0 || move_dir.y != 0) {
        SDL_Point next_pos = {player.get_position().x + move_dir.x, player.get_position().y + move_dir.y};
        bool blocked = false;

        for (size_t i = 0; i < collectibles.size(); ++i) {
            Gold* gold = dynamic_cast<Gold*>(collectibles[i]);
            if (gold &&
                gold->get_position().x == next_pos.x &&
                gold->get_position().y == next_pos.y) {
                
                if (gold->try_push(move_dir, *board, collectibles)) {
                    player.move(move_dir, *board);  // толкнули мешок и двинулись
                }
                blocked = true;
                break;
            }
        }

        if (!blocked) {
            player.move(move_dir, *board);  // просто двинулись
        }
    }

    // === Обновление всех collectible (включая падение золота) ===
    for (size_t i = 0; i < collectibles.size(); ) {
        collectibles[i]->update(*board);

        SDL_Point pos = collectibles[i]->get_position();

        bool collected = false;

        if (pos.x == player.get_position().x && pos.y == player.get_position().y) {
            if (auto emerald = dynamic_cast<Emerald*>(collectibles[i])) {
                player.add_score();
                cout << player.get_score() << endl;
                remove_collectible(i);
                collected = true;
            } else if (auto gold = dynamic_cast<Gold*>(collectibles[i])) {
                if (gold->get_is_broken()) {
                    player.add_score();
                    remove_collectible(i);
                    collected = true;
                } else {
                    player.death();
                    respawn();
                }
            }
        } else {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (enemies[j].get_position().x == pos.x && enemies[j].get_position().y == pos.y) {
                    if (auto gold = dynamic_cast<Gold*>(collectibles[i])) {
                        if (gold->get_is_broken()) {
                            remove_collectible(i);
                            collected = true;
                            break;
                        } else {
                            kill_enemy(j);
                            break;
                        }
                    }
                }
            }
        }

        if (!collected) {
            ++i;
        }
    }

        // === Обновление fireballs ===
    for (int i = 0; i < fireballs.size(); ) {
        fireballs[i].update(*board);

        // Проверка, активен ли fireball (не вышел за границы)
        if (!fireballs[i].is_active()) {
            fireballs.pop_back();
            continue;
        }

        SDL_Point fb_pos = fireballs[i].get_position();
        bool hit = false;

        for (size_t j = 0; j < enemies.size(); ++j) {
            SDL_Point enemy_pos = enemies[j].get_position();
            if (fb_pos.x == enemy_pos.x && fb_pos.y == enemy_pos.y) {
                kill_enemy(j);
                fireballs.pop_back();
                hit = true;
                break;
            }
        }

        if (!hit) {
            ++i;
        }
    }

    // === Обновление врагов ===
    for (auto& enemy : enemies) {
        enemy.update(*board, get_player_position());

        if (enemy.get_position().x == player.get_position().x &&
            enemy.get_position().y == player.get_position().y) {
            player.death();
            respawn();
        }
    }
}

SDL_Point Game::get_player_position() {
    return player.get_position();
}

Game::~Game() {
    deallocate();
}

void Game::deallocate() {
    delete board;
    delete board_renderer;
    for (size_t i = 0; i < collectibles.size(); i++) {
        delete collectibles[i];
    }
}