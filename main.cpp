#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Game.hpp"
#include "SimpleMenu.hpp"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        SDL_Log("SDL/TTF Init Error: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Digger Menu",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        SDL_Log("Font load failed: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    enum class AppState { Menu, Game, Exit };
    AppState state = AppState::Menu;

    while (state != AppState::Exit) {
        if (state == AppState::Menu) {
            SimpleMenu menu(renderer, font);
            MenuAction action = menu.run();
            if (action == MenuAction::StartGame) {
                state = AppState::Game;
            } else if (action == MenuAction::Exit) {
                state = AppState::Exit;
            }
        }

        if (state == AppState::Game) {
            Game game;
            if (game.init(renderer)) {
                GameResult result = game.run();
                if (result == GameResult::BackToMenu)
                    state = AppState::Menu;
                else if (result == GameResult::Quit)
                    state = AppState::Exit;
            } else {
                SDL_Log("Failed to initialize game");
                state = AppState::Menu;
            }
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
