#include <new>
#include "EditorTable/editortable.h"

static constexpr int BORDER_SPRITEBORDER_PADDING{10};

EditorTableBorder::EditorTableBorder(SpriteTableBorderType& stb, int rows, int cols, float sprite_size)
{
    if (rows < EDITOR_TABLEBORDER_MINROW || cols < EDITOR_TABLEBORDER_MINCOLS)
    {
#ifdef LOG
        std::cout << "Row or Cols less than minimum, cannot initiate.\n";
#endif
        init = false;
        return;
    }

    switch (stb.orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            // Ширина рамки редактора
            width = (cols * sprite_size);

            // Высота рамки редактора
            height = (rows * sprite_size);

            //bool wrongWidth{ (width + leftX) > bs.window_w};
            bool wrongWidth{(width + leftX) > WINDOW_W};
            if (wrongWidth)
            {
#ifdef LOG
                std::cout << "Widnow sizes: " << WINDOW_W << "x" << WINDOW_H << "\n";
                std::cout << "First calculated width: " << width << "\n";

                std::cout << "Width of border exceeds window width, and was"
                          << " adjusted.\n ";
#endif
                while (wrongWidth && (cols > EDITOR_TABLEBORDER_MINCOLS))
                {
                    --cols;
                    width = (cols * sprite_size);
#ifdef LOG
                    std::cout << "Adjusted width: " << width << "\n";
#endif
                    wrongWidth = (width + leftX)  > WINDOW_W;
                }
                
            }

            bool wrongHeight{(height + leftY + BORDER_SPRITEBORDER_PADDING +
                stb.spriteBorderSizes.horizontal.h) > WINDOW_H};
            if (wrongHeight)
            {
#ifdef LOG
                std::cout << "Height of border exceeds window height, and was"
                          << " adjusted.\n ";
#endif
                while(wrongHeight && rows > EDITOR_TABLEBORDER_MINROW)
                {
                    --rows;
                    height = (rows * sprite_size);
#ifdef LOG
                    std::cout << "Adjusted height: " << height << "\n";
#endif
                    wrongHeight = (height + leftY + BORDER_SPRITEBORDER_PADDING +
                        stb.spriteBorderSizes.horizontal.h) > WINDOW_H;
                }
            }

            // Устанавливаем рамку редактора
            border = {leftX, leftY, width, height};

            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            // Ширина рамки редактора
            width = (cols * sprite_size);

            // Высота рамки редактора
            height = (rows * sprite_size);

            bool wrongWidth{(width + leftX + BORDER_SPRITEBORDER_PADDING + 
                stb.spriteBorderSizes.vertical.w) > WINDOW_W};
            if (wrongWidth)
            {
#ifdef LOG
                std::cout << "Width of border exceeds window width, and was"
                          << " adjusted.\n ";
#endif
                while (wrongWidth && cols > EDITOR_TABLEBORDER_MINCOLS)
                {
                    --cols;
                    width = (cols * sprite_size);
                    wrongWidth = (width + leftX + BORDER_SPRITEBORDER_PADDING +
                        stb.spriteBorderSizes.vertical.w) > WINDOW_W;
                }
            }
            bool wrongHeight{height > WINDOW_H};
            if (wrongHeight)
            {
                #ifdef LOG
                std::cout << "Height of border exceeds window height, and was"
                          << " adjusted.\n ";
#endif
                while (wrongHeight && rows > EDITOR_TABLEBORDER_MINROW)
                {
                    --rows;
                    height = (rows * sprite_size);
                    wrongHeight = height > WINDOW_H;
                }
            }

            // Устанавливаем рамку редактора
            border = {leftX, leftY, width, height};
            break;
        }
        default:
        {
        }
    }
}

EditorTable::EditorTable(ESpriteBorderOrientation sbOrientation,
                         SpriteTableBorderType& stb,
                         int rows,
                         int cols,
                         bool isActive)
{
    thisTableIsActive = isActive;
    tableBorder = new (std::nothrow) 
        EditorTableBorder(stb, rows, cols, SPRITE_SIZE);
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
            stb.spriteBorderSizes.horizontal.x = tableBorder->GetBorder().x;
            stb.spriteBorderSizes.horizontal.y = tableBorder->GetBorder().y +
                                         tableBorder->GetBorder().h +
                                         BORDER_SPRITEBORDER_PADDING;
            stb.spriteBorderRect = stb.spriteBorderSizes.horizontal;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            // При вертикальной ориентации спрайтбордера, 
            // стартовая позиция таблицы спрайтов - СПРАВА от рамки редактора
            stb.spriteBorderSizes.vertical.x = tableBorder->GetBorder().x +
                                         tableBorder->GetBorder().w +
                                         BORDER_SPRITEBORDER_PADDING;
            stb.spriteBorderSizes.vertical.y = tableBorder->GetBorder().y;
            stb.spriteBorderRect = stb.spriteBorderSizes.vertical;
            break;
        }
        default:
        {
        }
    }



}


EditorTable::~EditorTable()
{
    delete tableBorder;
    tableBorder = nullptr;
}
