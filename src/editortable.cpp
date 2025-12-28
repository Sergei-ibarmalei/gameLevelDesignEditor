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

    tableBorder = std::make_unique<EditorTableBorder>(stb, rows, cols, SPRITE_SIZE);
    if (!tableBorder->init)
    {
#ifdef LOG
        std::cout << "Cannot create Editor table, abort.\n";
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
    size_t widing {10}; // widing назначаем для теста
    EditorTableTile_rows = static_cast<size_t>(tableBorder->GetTheRealRC().y);
    EditorTableTile_cols = static_cast<size_t>(tableBorder->GetTheRealRC().x) + widing;
#ifdef LOG
    std::cout << "Editor table tile array: [" << EditorTableTile_rows <<
        "][" << EditorTableTile_cols << "]\n";
#endif
    size_t editorTilesSize {EditorTableTile_rows * EditorTableTile_cols};
    editorTiles.assign(editorTilesSize, {-1});
   
}

void EditorTable::PutTextureOnTile(int row, int col, int atlasID)
{
    bool cursorIsOutOfRows {static_cast<size_t>(row) < 0 ||
        static_cast<size_t>(row) > EditorTableTile_rows};
    bool cursorIsOutOfCols {static_cast<size_t>(col) < 0 ||
        static_cast<size_t>(col) > EditorTableTile_cols};

    if (!cursorIsOutOfRows || !cursorIsOutOfCols)
    {
        editorTiles[static_cast<size_t>(row) * EditorTableTile_cols + startX +
            static_cast<size_t>(col)].tileId = atlasID;
    }

}



