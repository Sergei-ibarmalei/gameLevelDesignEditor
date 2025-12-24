#pragma once

#include "SpriteTable/consts.h"
#include "logs.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

constexpr int EDITOR_TABLEBORDER_MINROW{1};
constexpr int EDITOR_TABLEBORDER_MINCOLS{1};

struct BorderStuff
{
    SDL_Rect spriteBorder;
    ESpriteBorderOrientation orientation;
    int window_w;
    int window_h;
};

class EditorTableBorder
{
  private:
    int leftX{10};
    int leftY{10};
    int width{0};
    int height{0};
    bool init{true};
    bool active{false};
    SDL_Rect border{};

  public:
    EditorTableBorder(BorderStuff& bs, 
                      int rows = EDITOR_TABLEBORDER_MINROW,
                      int cols = EDITOR_TABLEBORDER_MINCOLS,
                      float sprite_size = SPRITE_SIZE);

    bool Status() const { return init; }
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    SDL_Rect GetBorder() const { return border; }
};

class EditorTable
{
  private:
      bool init {true};
      EditorTableBorder* tableBorder{nullptr};
  public:
    EditorTable(ESpriteBorderOrientation sbOrientation,
        int rows, int cols, int window_w, int window_h);

    EditorTable(const EditorTable&) = delete;
    EditorTable& operator=(const EditorTable&) = delete;
    EditorTable(EditorTable&&) = delete;
    bool Status() const { return init; }
};
