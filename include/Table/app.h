#pragma once

#include <SDL.h>
#include <SDL_image.h>

class App {
public:
    bool init();
    void run();
    void shutdown();

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool running_ = false;
};
