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
            bool wrongWidth{ (width + leftX) > bs.window_w};
            if (wrongWidth)
            {
#ifdef LOG
                std::cout << "Widnow sizes: " << bs.window_w << "x" << bs.window_h << "\n";
                std::cout << "First calculated width: " << width << "\n";

                std::cout << "Width of border exceeds window width, and was"
                          << " adjusted.\n ";
#endif
                while (wrongWidth && (cols > EDITOR_TABLEBORDER_MINCOLS))
                {
                    --cols;
                    width = static_cast<int>(cols * sprite_size);
#ifdef LOG
                    std::cout << "Adjusted width: " << width << "\n";
#endif
                    wrongWidth = (width + leftX)  > bs.window_w;
                }
                
            }
            bool wrongHeight {(height + leftY + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.horizontal.h) >
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
#ifdef LOG
                    std::cout << "Adjusted height: " << height << "\n";
#endif
                    wrongHeight = (height + leftY + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.horizontal.h) >
                                  bs.window_h;
                }
            }

            border = {leftX, leftY, width, height};
            // При горизонтальной ориентации спрайтбордера, он находится
            // ПОД рамкой редактора
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            width = static_cast<int>(cols * sprite_size);
            height = static_cast<int>(rows * sprite_size);
            bool wrongWidth{(width + leftX + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.vertical.w) >
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
                    wrongWidth = (width + leftX + BORDER_SPRITEBORDER_PADDING + bs.spriteBorder.vertical.w) >
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
    USpriteBorderSizes spriteBorder,
    int rows, int cols, bool isActive)
{
    thisTableIsActive = isActive;
    BorderStuff bs{};
    bs.spriteBorder = spriteBorder;
    bs.orientation = sbOrientation;
    bs.window_w = WINDOW_W;
    bs.window_h = WINDOW_H;
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
    switch (sbOrientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            // При горизонтальной ориентации спрайтбордера, 
            // стартовая позиция таблицы спрайтов - ПОД рамкой редактора
            spriteBorder.horizontal.x = tableBorder->GetBorder().x;
            spriteBorder.horizontal.y = tableBorder->GetBorder().y +
                                         tableBorder->GetBorder().h +
                                         BORDER_SPRITEBORDER_PADDING;
            madeSpriteBorder.x = spriteBorder.horizontal.x;
            madeSpriteBorder.y = spriteBorder.horizontal.y;
            madeSpriteBorder.w = spriteBorder.horizontal.w;
            madeSpriteBorder.h = spriteBorder.horizontal.h;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            // При вертикальной ориентации спрайтбордера, 
            // стартовая позиция таблицы спрайтов - СПРАВА от рамки редактора
            spriteBorder.vertical.x = tableBorder->GetBorder().x +
                                         tableBorder->GetBorder().w +
                                         BORDER_SPRITEBORDER_PADDING;
            spriteBorder.vertical.y = tableBorder->GetBorder().y;
            madeSpriteBorder.x = spriteBorder.vertical.x;
            madeSpriteBorder.y = spriteBorder.vertical.y;
            madeSpriteBorder.w = spriteBorder.vertical.w;
            madeSpriteBorder.h = spriteBorder.vertical.h;
            break;
        }
        default:
        {
        }
    }



}

//SDL_Rect EditorTable::madeSpriteBorder()
//{
//    SDL_Rect sb{};
//    sb.x = bs.sprite
//}

EditorTable::~EditorTable()
{
    delete tableBorder;
    tableBorder = nullptr;
}
