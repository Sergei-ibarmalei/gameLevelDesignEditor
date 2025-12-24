#include "EditorTable/editortable.h"

EditorTableBorder::EditorTableBorder(int window_w, int window_h,
    int row, int cols, float sprite_size)
{
    if (row < EDITOR_TABLEBORDER_MINROW || cols < EDITOR_TABLEBORDER_MINCOLS)
    {
#ifdef LOG
        std::cout << "Row or Cols less than minimum, cannot initiate.\n";
#endif
        init = false;
        return;
    }
    width = static_cast<int>(cols * sprite_size);
    height = static_cast<int>(row * sprite_size);

    if (width > window_w || height > window_h)
    {
#ifdef LOG
        std::cout << "Table border size exceeds window size and will be adjusted.\n";
#endif
        widht =
    }
    border.x = leftX;
    border.y = leftY;
    border.w = width;
    border.h = height;
}

EditorTable::EditorTable(int window_w, int window_h)
{
    tableBorder = new EditorTableBorder()
}
