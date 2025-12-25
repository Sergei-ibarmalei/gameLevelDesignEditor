#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "SpriteTable/spritetable.h"
#include "EditorTable/editortable.h"
#include "position.h"

class App
{
  public:
    // USpriteBorderSizes spriteTableBorderSizes;
    SpriteTableBorderType spriteTableBorder;
    bool init(int w, int h);
    void run();
    void shutdown();
    void defineSpriteBorderSizes(ESpriteBorderOrientation orientation,
        SpriteTableBorderType& stb);

  private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    bool running_ = false;

    Uint32 lastTime{0};
    Uint32 currentTime{0};
    float deltaTime{0.0f};
};
