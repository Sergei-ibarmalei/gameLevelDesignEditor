#pragma once

#include "SpriteTable/spritetable.h"
#include <SDL.h>
#include <SDL_image.h>

class App
{
  public:
    bool init(int w, int h);
    void run();
    void shutdown();

  private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool running_ = false;

    Uint32 lastTime{0};
    Uint32 currentTime{0};
    float deltaTime{0.0f};
};
