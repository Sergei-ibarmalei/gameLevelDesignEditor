#include <new>
#include "EditorTable/editortable.h"

static constexpr int BORDER_SPRITEBORDER_PADDING{10};

EditorTableBorder::EditorTableBorder(BorderStuff& bs, int rows, int cols, float sprite_size)
{
    if (rows < EDITOR_TABLEBORDER_MINROW || cols < EDITOR_TABLEBORDER_MINCOLS)
    {
#ifdef LOG
        std::cout << "Row or Cols less than minimum, cannot initiate.\n";
#endif
        init = false;
        return;
    }

    switch (bs.orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            width = static_cast<int>(cols * sprite_size);
            height = static_cast<int>(rows * sprite_size);
            bool wrongWidth{width > bs.window_h};
            if (wrongWidth)
            {
#ifdef LOG
                std::cout << "Width of border exceeds window width, and was"
                          << " adjusted.\n ";
#endif
                while (wrongWidth && cols > EDITOR_TABLEBORDER_MINCOLS)
                {
                    --cols;
                    width = static_cast<int>(cols * sprite_size);
                    wrongWidth = width > bs.window_w;
                }
                
            }
            bool wrongHeight {(height + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.h) >
                             bs.window_h};
            if (wrongHeight)
            {
#ifdef LOG
                std::cout << "Height of border exceeds window height, and was"
                          << " adjusted.\n ";
#endif
                while(wrongHeight && rows > EDITOR_TABLEBORDER_MINROW)
                {
                    --rows;
                    height = static_cast<int>(rows * sprite_size);
                    wrongHeight = (height + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.h) >
                                  bs.window_h;
                }
            }

            border = {leftX, leftY, width, height};
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            width = static_cast<int>(cols * sprite_size);
            height = static_cast<int>(rows * sprite_size);
            bool wrongWidth{(width + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.w) >
                            bs.window_w};
            if (wrongWidth)
            {
#ifdef LOG
                std::cout << "Width of border exceeds window width, and was"
                          << " adjusted.\n ";
#endif
                while (wrongWidth && cols > EDITOR_TABLEBORDER_MINCOLS)
                {
                    --cols;
                    width = static_cast<int>(cols * sprite_size);
                    wrongWidth = (width + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.w) >
                                 bs.window_w;
                }
            }
            bool wrongHeight{height > bs.window_h};
            if (wrongHeight)
            {
                #ifdef LOG
                std::cout << "Height of border exceeds window height, and was"
                          << " adjusted.\n ";
#endif
                while (wrongHeight && rows > EDITOR_TABLEBORDER_MINROW)
                {
                    --rows;
                    height = static_cast<int>(rows * sprite_size);
                    wrongHeight = height > bs.window_h;
                }
            }
            border = {leftX, leftY, width, height};
            break;
        }
        default:
        {
        }
    }
}

EditorTable::EditorTable(ESpriteBorderOrientation sbOrientation,
    int rows, int cols, int window_w, int window_h, bool isActive)
{
    thisTableIsActive = isActive;
    BorderStuff bs{};
    bs.orientation = sbOrientation;
    bs.window_w = window_w;
    bs.window_h = window_h;
    tableBorder = new (std::nothrow) 
        EditorTableBorder(bs, rows, cols, SPRITE_SIZE);
    if (!tableBorder)
    {
#ifdef LOG
        std::cout << "Cannot allocate memory for EditorTableBorder, abort.\n";
#endif
        init = false;
        return;
    }
    if (!tableBorder->Status())
    {
#ifdef LOG
        std::cout << "Cannot initiate EditorTableBorder, abort.\n";
#endif
        init = false;
        return;
    }



}

EditorTable::~EditorTable()
{
    delete tableBorder;
    tableBorder = nullptr;
}
