#include <new>
#include "EditorTable/editortable.h"

static constexpr int BORDER_SPRITEBORDER_PADDING{10};

EditorTableBorder::EditorTableBorder(SpriteTableBorderType& stb,
                                     int rows,
                                     int cols,
                                     float sprite_size)
{
    if (rows < EDITOR_TABLEBORDER_MINROW || cols < EDITOR_TABLEBORDER_MINCOLS)
    {
#ifdef LOG
        std::cout << "Row or Cols less than minimum, cannot initiate.\n";
#endif
        init = false;
        return;
    }

    // Ширина рамки редактора
    width = (cols * sprite_size);

    // Высота рамки редактора
    height = (rows * sprite_size);

    switch (stb.orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {

            // bool wrongWidth{ (width + leftX) > bs.window_w};
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
                    wrongWidth = (width + leftX) > WINDOW_W;
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
                while (wrongHeight && rows > EDITOR_TABLEBORDER_MINROW)
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

            // Реальное количество rows cols нам необходимо для расчетов 
            // класса helperdot
            theRealRowsAndCols.x = cols;
            theRealRowsAndCols.y = rows;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {

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
            bool wrongHeight{height + leftY > WINDOW_H};
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
                    wrongHeight = height + leftY > WINDOW_H;
                }
            }
            theRealRowsAndCols.x = cols;
            theRealRowsAndCols.y = rows;
            break;
        }
        default:
        {
        }
    }

    // Устанавливаем рамку редактора
    border = {leftX, leftY, width, height};
    intBorder = {static_cast<int>(leftX),
                 static_cast<int>(leftY),
                 static_cast<int>(width),
                 static_cast<int>(height)};
}

EditorTable::EditorTable(ESpriteBorderOrientation sbOrientation,
                         SpriteTableBorderType& stb,
                         int rows,
                         int cols,
                         bool isActive)
{
    thisTableIsActive = isActive;
    tableBorder = new (std::nothrow) EditorTableBorder(stb, rows, cols, SPRITE_SIZE);
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
            stb.spriteBorderSizes.horizontal.x = static_cast<int>(tableBorder->GetBorder().x);
            stb.spriteBorderSizes.horizontal.y =
                static_cast<int>(tableBorder->GetBorder().y + tableBorder->GetBorder().h +
                                 BORDER_SPRITEBORDER_PADDING);
            stb.spriteBorderRect = stb.spriteBorderSizes.horizontal;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            // При вертикальной ориентации спрайтбордера,
            // стартовая позиция таблицы спрайтов - СПРАВА от рамки редактора
            stb.spriteBorderSizes.vertical.x =
                static_cast<int>(tableBorder->GetBorder().x + tableBorder->GetBorder().w +
                                 BORDER_SPRITEBORDER_PADDING);
            stb.spriteBorderSizes.vertical.y = static_cast<int>(tableBorder->GetBorder().y);
            stb.spriteBorderRect = stb.spriteBorderSizes.vertical;
            break;
        }
        default:
        {
        }
    }
    size_t widing {10};
    EditorTableTile_rows = static_cast<size_t>(tableBorder->GetTheRealRC().y);
    size_t EditorTableTile_cols = static_cast<size_t>(tableBorder->GetTheRealRC().x) + widing;
#ifdef LOG
    std::cout << "Editor table tile array: [" << EditorTableTile_rows <<
        "][" << EditorTableTile_cols << "]\n";
    EditorTiles.reserve(EditorTableTile_rows * EditorTableTile_cols);
#endif

    
}

void EditorTable::PutTextureOnTile(int row, int col, int atlasID)
{
    //if (row < 0 || col < 0) return;
    EditorTiles.at(static_cast<size_t>(row) * EditorTableTile_cols + startX +
        static_cast<size_t>(col)).tileId = atlasID;
}

EditorTable::~EditorTable()
{
    delete tableBorder;
    tableBorder = nullptr;
}

