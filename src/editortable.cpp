#include <new>
#include "EditorTable/editortable.h"

static constexpr int BORDER_SPRITEBORDER_PADDING{10};

EditorTableBorder::EditorTableBorder(SpriteTableBorderType& stb,
                                     ArraySizes& theSlice,
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
            //theRealRowsAndCols.x = cols;
            //theRealRowsAndCols.y = rows;


            // Определяем размеры "среза" вектора спрайтов для вывода на экран
            // Размеры среза не в пикселях экрана, а в количестве строк и столбцов
            // срез равняется размерам рабочей области EditorTable, поэтому
            // width = cols, height = rows
            //theRealRowsAndCols.width = static_cast<size_t>(cols);
            //theRealRowsAndCols.height = static_cast<size_t>(rows);
            theSlice.rows = static_cast<size_t>(rows);
            theSlice.cols = static_cast<size_t>(cols);

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
            //theRealRowsAndCols.x = cols;
            //theRealRowsAndCols.y = rows;
            //theRealRowsAndCols.width = static_cast<size_t>(cols);
            //theRealRowsAndCols.height = static_cast<size_t>(rows);
            theSlice.cols = static_cast<size_t>(cols);
            theSlice.rows = static_cast<size_t>(rows);
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

    tableBorder = std::make_unique<EditorTableBorder>(stb, realAndSliceRowCol.slice,
                            rows, cols, SPRITE_SIZE);
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
    size_t widingCols {10}; // widing назначаем для теста
    realAndSliceRowCol.real.rows = realAndSliceRowCol.slice.rows;
    realAndSliceRowCol.real.cols = realAndSliceRowCol.slice.cols + widingCols;
#ifdef LOG
    std::cout << "Editor table tile array: [" << realAndSliceRowCol.real.rows <<
        "][" << realAndSliceRowCol.real.cols << "]\n";
#endif
    size_t editorTilesSize {realAndSliceRowCol.real.cols * 
        realAndSliceRowCol.real.rows};
    editorTiles.editorTilesVector.assign(editorTilesSize, {-1});
   
}

bool EditorTable::checkIsCursorValid(int row, int col, size_t& idx)
{
    if (row < 0 || col < 0) return false;
    const auto sr = realAndSliceRowCol.slice.rows;
    const auto sc = realAndSliceRowCol.slice.cols;

    if (static_cast<size_t>(row) >= sr) return false;
    if (static_cast<size_t>(col) >= sc) return false;

    idx = static_cast<size_t>(row) * realAndSliceRowCol.real.cols +
        static_cast<size_t>(editorTiles.startX) +
        static_cast<size_t>(col);
    if (idx >= editorTiles.editorTilesVector.size()) return false;
    return true;
}

void EditorTable::PutTextureOnTile(int row, int col, int atlasID)
{

    size_t idx {};
    if (!checkIsCursorValid(row, col, idx)) return;
     editorTiles.editorTilesVector[idx].tileId = atlasID;
}

void EditorTable::RemoveTextureOnTile(int row, int col)
{
    size_t idx {};
    if (!checkIsCursorValid(row, col, idx)) return;
    editorTiles.editorTilesVector[idx].tileId = -1;

}





