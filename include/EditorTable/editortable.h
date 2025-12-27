#pragma once

#include "SpriteTable/consts.h"
#include "logs.h"
#include "video.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

constexpr int EDITOR_TABLEBORDER_MINROW{1};
constexpr int EDITOR_TABLEBORDER_MINCOLS{1};
constexpr float EDITOR_TABLEBORDER_LEFTX{10.f};
constexpr float EDITOR_TABLEBORDER_LEFTY{10.f};

struct Tile
{
    int tileId {-1};
};

// Рамка редактора
struct EditorTableBorder
{
    float leftX{EDITOR_TABLEBORDER_LEFTX};
    float leftY{EDITOR_TABLEBORDER_LEFTY};
    float width{0};
    float height{0};
    bool init{true};
    bool active{false};
    SDL_Point theRealRowsAndCols {0, 0};
    SDL_FRect border{};
    SDL_Rect intBorder{};

    EditorTableBorder(SpriteTableBorderType& stb,
                      int rows = EDITOR_TABLEBORDER_MINROW,
                      int cols = EDITOR_TABLEBORDER_MINCOLS,
                      float sprite_size = SPRITE_SIZE);
    bool Status() const { return init; }
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    const SDL_FRect& GetBorder() const { return border; }
    const SDL_Rect& GetIntBorder() const { return intBorder; }
    const SDL_Point& GetTheRealRC() const {return theRealRowsAndCols;}
};

class EditorTable
{
  private:
    bool init{true};
    bool thisTableIsActive{false};
    std::vector<Tile> EditorTiles;
    EditorTableBorder* tableBorder{nullptr};

  public:
    EditorTable(ESpriteBorderOrientation sbOrientation,
                SpriteTableBorderType& stb,
                int rows,
                int cols,
                bool isActive = true);
    ~EditorTable();
    EditorTable(const EditorTable&) = delete;
    EditorTable& operator=(const EditorTable&) = delete;
    EditorTable(EditorTable&&) = delete;
    bool Status() const { return init; }
    const bool IsActive() const { return thisTableIsActive; }
    void SetActive(bool a) { thisTableIsActive = a; }
    const SDL_FRect& GetTableBorder() const
    {
            return tableBorder->GetBorder();
    }
    const SDL_Rect& GetIntTableBorder() const
    {
            return tableBorder->GetIntBorder(); 
    }
    const SDL_Point& GetRealRowsColsEditorTable() const {return tableBorder->GetTheRealRC(); }
};
