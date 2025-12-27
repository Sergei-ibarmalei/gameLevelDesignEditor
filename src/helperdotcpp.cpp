#include "Helper/helperdot.h"

static const char* helperDotPath = "assets/helperDot_1.png";


HelperDot::HelperDot(
        const SDL_Rect& border,
        const SDL_Point& realRowsColsNomber)
{
    initAllHelperDotsDestRects(border, realRowsColsNomber);
 
}



void HelperDot::initAllHelperDotsDestRects(const SDL_Rect& border,
    const SDL_Point& realRowsColsNomber)
{
    int startx = border.x + static_cast<int>(SPRITE_SIZE);
    int starty = border.y + static_cast<int>(SPRITE_SIZE);

    
    size_t dotRows {static_cast<size_t>(realRowsColsNomber.y - 1)};
    size_t dotCols {static_cast<size_t>(realRowsColsNomber.x - 1)};
    
    allHelperDots.reserve(dotRows * dotCols);
    for (size_t r = 0; r < dotRows; r++)
    {
        for (size_t c = 0; c < dotCols; c++)
        {
            allHelperDots.emplace_back(startx + static_cast<int>(c * SPRITE_SIZE) -
                HELPDOT_SPRITESIZE / 2, starty + static_cast<int>(r * SPRITE_SIZE) -
                HELPDOT_SPRITESIZE / 2);
        }
    }

}
