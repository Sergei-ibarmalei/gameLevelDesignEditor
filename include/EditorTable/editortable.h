#pragma once

#include "SpriteTable/consts.h"
#include "logs.h"
#include "video.h"
#include "slice.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>

constexpr int EDITOR_TABLEBORDER_MINROW{1};
constexpr int EDITOR_TABLEBORDER_MINCOLS{1};
constexpr float EDITOR_TABLEBORDER_LEFTX{10.f};
constexpr float EDITOR_TABLEBORDER_LEFTY{10.f};

struct Tile
{
    int tileId {-1};
};

struct EditorTiles
{
    int startX {0};
    std::vector<Tile> editorTilesVector;
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
    SDL_FRect border{};
    SDL_Rect intBorder{};

    EditorTableBorder(SpriteTableBorderType& stb,
                      ArraySizes& theSlice,
                      int rows = EDITOR_TABLEBORDER_MINROW,
                      int cols = EDITOR_TABLEBORDER_MINCOLS,
                      float sprite_size = SPRITE_SIZE);
    ~EditorTableBorder() = default;
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    const SDL_FRect& GetBorder() const { return border; }
    const SDL_Rect& GetIntBorder() const { return intBorder; }
};



class EditorTable
{
  private:
    bool init{true};
    bool thisTableIsActive{false};
    EditorTiles editorTiles;
    std::unique_ptr<EditorTableBorder> tableBorder;
    RealAndSliceRowCol realAndSliceRowCol {};
    [[nodiscard]] bool checkIsCursorValid(int row, int col, size_t& idx);

  public:
    EditorTable(ESpriteBorderOrientation sbOrientation,
                SpriteTableBorderType& stb,
                int rows,
                int cols,
                bool isActive = true);
    ~EditorTable() = default;
    EditorTable(const EditorTable&) = delete;
    EditorTable& operator=(const EditorTable&) = delete;
    EditorTable(EditorTable&&) = delete;
    bool Status() const { return init; }
    bool IsActive() const { return thisTableIsActive; }
    void SetActive(bool a) { thisTableIsActive = a; }
    const EditorTiles& GetTiles()  const {return editorTiles;}         
    const SDL_FRect& GetTableBorder() const
    {
            return tableBorder->GetBorder();
    }
    const SDL_Rect& GetIntTableBorder() const
    {
            return tableBorder->GetIntBorder(); 
    }
    const RealAndSliceRowCol& GetRealAndSliseRowCol() const {return realAndSliceRowCol;}
    void PutTextureOnTile(int row, int col, int atlasID);
    void RemoveTextureOnTile(int row, int col);
    const EditorTiles& GetEditorTiles() const  {return editorTiles;}
    void MoveCanvasLeft();
    void MoveCanvasRight();
};


