#pragma once

#include "SpriteTable/consts.h"
#include "logs.h"
#include "video.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

constexpr int EDITOR_TABLEBORDER_MINROW{1};
constexpr int EDITOR_TABLEBORDER_MINCOLS{1};
constexpr int EDITOR_TABLEBORDER_LEFTX {10};
constexpr int EDITOR_TABLEBORDER_LEFTY{10};

struct BorderStuff
{
    USpriteBorderSizes spriteBorder;
    ESpriteBorderOrientation orientation;
    int window_w;
    int window_h;
};

struct EditorTableBorder
{
    int leftX{EDITOR_TABLEBORDER_LEFTX};
    int leftY{EDITOR_TABLEBORDER_LEFTY};
    int width{0};
    int height{0};
    bool init{true};
    bool active{false};
    SDL_Rect border{};

    EditorTableBorder(BorderStuff& bs,
                      int rows = EDITOR_TABLEBORDER_MINROW,
                      int cols = EDITOR_TABLEBORDER_MINCOLS,
                      float sprite_size = SPRITE_SIZE);
    bool Status() const { return init; }
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    const SDL_Rect& GetBorder() const { return border; }
};

class EditorTable
{
  private:
    bool init{true};
    bool thisTableIsActive{false};
    //SDL_Point spriteTableStartPosition{0, 0};
    EditorTableBorder* tableBorder{nullptr};
    SDL_Rect madeSpriteBorder{};

  public:
    EditorTable(ESpriteBorderOrientation sbOrientation,
                USpriteBorderSizes spriteBorderSizes,
                int rows,
                int cols,
                bool isActive = true);
    ~EditorTable();
    EditorTable(const EditorTable&) = delete;
    EditorTable& operator=(const EditorTable&) = delete;
    EditorTable(EditorTable&&) = delete;
    bool Status() const { return init; }
    const SDL_Rect& GetTableBorder() const
    {
        if (tableBorder)
            return tableBorder->GetBorder();
        return SDL_Rect{};
    }
    //const SDL_Point& GetSpriteTableStartPosition() const { return spriteTableStartPosition; }
    SDL_Rect GetMadeSpriteBorder() const { return madeSpriteBorder; }
};
