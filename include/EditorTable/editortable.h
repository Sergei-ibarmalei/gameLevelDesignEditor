#pragma once

#include <SDL.h>
#include <SDL_image.h>


class EditorTableBorder
{
private:
    int leftX {10};
    int leftY {10};
    int width {0};
    int height {0};
public:
    EditorTableBorder(int row, int cols, int sprite_size);
};

class EditorTable
{
  private:
  public:
    EditorTable(int window_w, int window_h);

    EditorTable(const EditorTable&) = delete;
    EditorTable& operator=(const EditorTable&) = delete;
    EditorTable(EditorTable&&) = delete;
};
