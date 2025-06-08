#include "SimpleMenu.hpp"

SimpleMenu::SimpleMenu(SDL_Renderer* renderer, TTF_Font* font)
    : renderer(renderer), font(font) {}

MenuAction SimpleMenu::run() {
    SDL_Event event;
    bool quit = false;
    MenuAction action = MenuAction::Exit;   

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                if (confirmExitDialog()) {
                    quit = true;
                    action = MenuAction::Exit;
                }
            } else {
                handleInput(event, quit, action);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return action;
}

void SimpleMenu::render() {
    SDL_Color colorNormal = {255, 255, 255, 255};
    SDL_Color colorSelected = {255, 255, 0, 255};

    int y = 200;
    for (int i = 0; i < items.size(); ++i) {
        SDL_Color color = (i == selectedIndex) ? colorSelected : colorNormal;
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, items[i].c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect dst = {400 - textW / 2, y, textW, textH};
        SDL_RenderCopy(renderer, texture, nullptr, &dst);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        y += 60;
    }
}

bool SimpleMenu::confirmExitDialog() {
    bool running = true;
    bool confirmed = false;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_y) {
                    confirmed = true;
                    running = false;
                } else if (event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render_text("Вы действительно хотите выйти?", 200, 250);
        render_text("Y - Да    N - Нет", 300, 300);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return confirmed;
}

void SimpleMenu::render_text(const std::string& text, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void SimpleMenu::handleInput(SDL_Event& event, bool& quit, MenuAction& action) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
                break;
            case SDLK_DOWN:
                selectedIndex = (selectedIndex + 1) % items.size();
                break;
            case SDLK_RETURN:
                if (selectedIndex == 0) {
                    quit = true;
                    action = MenuAction::StartGame;
                } else if (selectedIndex == 1) {
                    if (confirmExitDialog()) {
                        quit = true;
                        action = MenuAction::Exit;
                    }
                }
                break;
        }
    }
}
