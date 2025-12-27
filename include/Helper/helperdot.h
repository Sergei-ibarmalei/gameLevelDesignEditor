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
    SDL_Rect helperDotSource{0, 0, HELPDOT_SPRITESIZE, HELPDOT_SPRITESIZE};
    std::vector<SDL_Point>allHelperDots;

    void initAllHelperDotsDestRects(const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);

public:
    HelperDot(const SDL_Rect& border, 
        const SDL_Point& realRowsColsNomber);
    ~HelperDot() = default;
    HelperDot(const HelperDot&) = delete;
    HelperDot& operator=(const HelperDot&) = delete;
    HelperDot(HelperDot&&) = delete;
    HelperDot& operator=(HelperDot&&) = delete;

    bool Status() const {return init;}
    const std::vector<SDL_Point>& GetHelperDots() const {return allHelperDots;}

  
};
