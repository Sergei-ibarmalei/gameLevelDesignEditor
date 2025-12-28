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
    //SDL_Point theRealRowsAndCols {0, 0};
    //VectorSpriteSlice theRealRowsAndCols {};
    //ArraySizes theRealArrayOfTiles;
    //ArraySizes theSlice;
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
    //const SDL_Point& GetTheRealRC() const {return theRealRowsAndCols;}
    //const VectorSpriteSlice& GetSliseSizes() const {return theRealRowsAndCols;}
};



class EditorTable
{
  private:
    bool init{true};
    bool thisTableIsActive{false};
    //std::vector<Tile> editorTiles;
    EditorTiles editorTiles;
    std::unique_ptr<EditorTableBorder> tableBorder;
    //size_t startX {0};
    //size_t EditorTableTile_rows {0}; // полное количество row в создаваемом полотне (editorTiles.editorTilesVector)
    //size_t EditorTableTile_cols {0}; // полное количество col в создаваемом полотне
    //ArraySizes theRealArrayOfTiles;
    //ArraySizes theSlice;
    RealAndSliceRowCol realAndSliceRowCol {};

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
    //const SDL_Point& GetRealRowsColsEditorTable() const {return tableBorder->GetTheRealRC(); }

    // возвращаем реальное количество rows и cols в editorTable
    //const VectorSpriteSlice& GetSliceSizes() const {return tableBorder->GetSliseSizes(); }
    const RealAndSliceRowCol& GetRealAndSliseRowCol() const {return realAndSliceRowCol;}
    void PutTextureOnTile(int row, int col, int atlasID);
    const EditorTiles& GetEditorTiles() const  {return editorTiles;}
};


