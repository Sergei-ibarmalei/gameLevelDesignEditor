#include "Helper/helperdot.h"

static const char* helperDotPath = "assets/helperDot.png";


HelperDot::HelperDot(SDL_Renderer* renderer)
{
    if (!renderer)
    {
#ifdef LOG
        std::cout << "Cannot initate helper dot, renderer is absent. Abort.\n";
#endif
        init = false;
        return;
    }

    init = makeTexture(renderer);

}

bool HelperDot::makeTexture(SDL_Renderer* renderer)
{
    SDL_Surface* tmpHelperSurface = IMG_Load(helperDotPath);
    if (!tmpHelperSurface)
    {
#ifdef LOG
        std::cout << "Could not create surface from: " << helperDotPath <<
            ", abort.\n";
#endif
        return false;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0,
        HELPERDOT_SPRITESIZE, HELPERDOT_SPRITESIZE, 32,
        SDL_PIXELFORMAT_RGBA32);

    if (!surface)
    {
#ifdef LOG
        std::cout << "Could not create RGB surface from : " <<
            helperDotPath << ", abort.\n";
#endif
        SDL_FreeSurface(tmpHelperSurface);
        tmpHelperSurface = nullptr;
        return false;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    SDL_SetSurfaceBlendMode(tmpHelperSurface, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(tmpHelperSurface, NULL, surface, &helperDotSource);
    SDL_FreeSurface(tmpHelperSurface);
    tmpHelperSurface = nullptr;

    helperDotTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!helperDotTexture)
    {
        SDL_FreeSurface(surface);
        surface = nullptr;
#ifdef LOG
        std::cout << "Could not create helper dot texture, abort.\n";
#endif
        return false;
    }
    SDL_FreeSurface(surface);
    surface = nullptr;

    SDL_SetTextureBlendMode(helperDotTexture, SDL_BLENDMODE_BLEND);
    return true;
}

HelperDot::~HelperDot()
{
    if (helperDotTexture)
    {
        SDL_DestroyTexture(helperDotTexture);
        helperDotTexture = nullptr;
    }
}

void HelperDot::InitAllHelperDotsDestRects(const SDL_Rect& border,
    int rows, int cols)
{

}
