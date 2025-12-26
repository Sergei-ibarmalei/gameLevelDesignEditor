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
    //bool makeTexture(SDL_Renderer* r);
    SDL_Rect helperDotSource{0, 0, HELPDOT_SPRITESIZE, HELPDOT_SPRITESIZE};
    //std::vector<SDL_Rect> allHelperDotsDestRects;
    //SDL_Texture* helperDotTexture {nullptr};
    std::vector<SDL_Point>allHelperDots;

    void initAllHelperDotsDestRects(const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);

public:
    HelperDot(const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);
    ~HelperDot();
    HelperDot(const HelperDot&) = delete;
    HelperDot& operator=(const HelperDot&) = delete;
    HelperDot(HelperDot&&) = delete;
    HelperDot& operator=(HelperDot&&) = delete;

    bool Status() const {return init;}
    //SDL_Texture* GetHelperDotTexture() const {return helperDotTexture;}
    //const std::vector<SDL_Rect>& GetHelperSourceRects() const {return allHelperDotsDestRects;}
    const std::vector<SDL_Point>& GetHelperDots() const {return allHelperDots;}

  
};
