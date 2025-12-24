#include "EditorTable/editortable.h"

EditorTableBorder::EditorTableBorder(int row, int cols, float sprite_size)
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
}
