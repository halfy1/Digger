#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

enum class MenuAction {
    StartGame,
    Exit
};

class SimpleMenu {
public:
    SimpleMenu(SDL_Renderer* renderer, TTF_Font* font);
    MenuAction run();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> items = {"Новая игра", "Выход"};
    int selectedIndex = 0;

    void render();
    void handleInput(SDL_Event& event, bool& quit, MenuAction& action);

    void render_text(const std::string& text, int x, int y); // добавим
    bool confirmExitDialog(); // <- новый метод
};
