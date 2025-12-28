#include "Helper/helperdot.h"


HelperDot::HelperDot(const SDL_Rect& border,
        const ArraySizes& slice)
{
    // slice - в данном случае это реальное количество row cols
    // в editorTable, т.е. на рабочем полотне
    // знание этих количеств помогает нам расчитать необходимое количество
    // впомогательных точек на экране
    initAllHelperDotsDestRects(border, slice);
 
}



void HelperDot::initAllHelperDotsDestRects(const SDL_Rect& border,
    const ArraySizes& slice)
{
    if (slice.rows <= 0 || slice.cols <= 0)
    {
#ifdef LOG
        std::cout << "Cannot create helper dot class, real rows or cols are less " <<
            " than zero, abort.\n";
#endif
        init = false;
        return;
    }

    int startx = border.x + static_cast<int>(SPRITE_SIZE);
    int starty = border.y + static_cast<int>(SPRITE_SIZE);

    
    size_t dotRows {static_cast<size_t>(slice.rows - 1)};
    size_t dotCols {static_cast<size_t>(slice.cols - 1)};
    
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
