#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "SpriteTable/consts.h"
#include "logs.h"


class HelperDot
{
private:
    bool init {true};
    bool makeTexture(SDL_Renderer* r);
    SDL_Rect helperDotSource{0, 0, HELPERDOT_SPRITESIZE, HELPERDOT_SPRITESIZE};
    std::vector<SDL_Rect> allHelperDotsDestRects;
    SDL_Texture* helperDotTexture {nullptr};

public:
    HelperDot(SDL_Renderer* render);
    ~HelperDot();
    HelperDot(const HelperDot&) = delete;
    HelperDot& operator=(const HelperDot&) = delete;
    HelperDot(HelperDot&&) = delete;
    HelperDot& operator=(HelperDot&&) = delete;

    bool Status() const {return init;}
    SDL_Texture* GetHelperDotTexture() const {return helperDotTexture;}
    void InitAllHelperDotsDestRects(const SDL_Rect& border, int rows, int cols);
};
