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

    void initAllHelperDotsDestRects(const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);

public:
    HelperDot(SDL_Renderer* render, const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);
    ~HelperDot();
    HelperDot(const HelperDot&) = delete;
    HelperDot& operator=(const HelperDot&) = delete;
    HelperDot(HelperDot&&) = delete;
    HelperDot& operator=(HelperDot&&) = delete;

    bool Status() const {return init;}
    SDL_Texture* GetHelperDotTexture() const {return helperDotTexture;}
    const std::vector<SDL_Rect>& GetHelperSourceRects() const {return allHelperDotsDestRects;}
    SDL_Texture* GetTexture() const {return helperDotTexture;}
  
};
