#include "Table/app.h"
#include <iostream>

static void showBorder(SDL_Renderer* renderer);
static void showSimpleSpriteVector(SDL_Renderer* renderer, SDL_Texture* texture,
                                   const std::vector<Sprite>& vectorSprite);

bool App::init(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window_ = SDL_CreateWindow("Table", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_SHOWN);

    if (!window_)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(
        window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    running_ = true;
    return true;
}

void App::run()
{
    SpriteTable spriteTable(renderer_);
    if (!spriteTable.Status())
        return;

    while (running_)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running_ = false;
        }

        SDL_SetRenderDrawColor(renderer_, 30, 30, 36, 255);
        SDL_RenderClear(renderer_);
        SDL_RenderPresent(renderer_);
    }
}

void App::shutdown()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}

static void showBorder(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0);
    SDL_RenderDrawRect(renderer, &BORDER_INT);
}

static void showSimpleSpriteVector(SDL_Renderer* renderer, SDL_Texture* texture,
                                   const std::vector<Sprite>& vectorSprite)
{
    for (auto& sprite : vectorSprite)
    {
        SDL_RenderCopyF(renderer, texture, &sprite.SourceRect(),
                        &sprite.transform.GetRect());
    }
}
