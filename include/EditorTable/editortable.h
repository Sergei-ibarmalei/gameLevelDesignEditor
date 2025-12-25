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

//struct BorderStuff
//{
//    USpriteBorderSizes spriteTableBorderSizes;
//    ESpriteBorderOrientation orientation;
//    int window_w; // размеры рабочего окна по ширине
//    int window_h; // размеры рабочего окна по высоте
//};

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

    EditorTableBorder(SpriteTableBorderType& stb,
                      int rows = EDITOR_TABLEBORDER_MINROW,
                      int cols = EDITOR_TABLEBORDER_MINCOLS,
                      float sprite_size = SPRITE_SIZE);
    bool Status() const { return init; }
    bool IsActive() const { return active; }
    void SetActive(bool a) { active = a; }
    const SDL_FRect& GetBorder() const { return border; }
};

class EditorTable
{
  private:
    bool init{true};
    bool thisTableIsActive{false};
    // SDL_Point spriteTableStartPosition{0, 0};
    EditorTableBorder* tableBorder{nullptr};
    //SDL_FRect fullMadeSpriteTableBorder{};

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
    const SDL_FRect& GetTableBorder() const
    {
            return tableBorder->GetBorder();
    }
    // const SDL_Point& GetSpriteTableStartPosition() const { return spriteTableStartPosition; }
    //const SDL_FRect& GetMadeSpriteTableBorder() const { return fullMadeSpriteTableBorder; }
};
